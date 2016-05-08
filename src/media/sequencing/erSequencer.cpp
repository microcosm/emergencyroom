#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player){
    network = _network;
    loader = _loader;
    player = _player;

    currentCollection = "";
    currentCollectionIndex = -1;
    queuesLoaded = false;
    collectionsLoaded = false;

    currentChannel = 1;
    translater = network->getTranslater();

    ofAddListener(ofEvents().update, this, &erSequencer::update);
    ofAddListener(network->clientMessageReceived(), this, &erSequencer::messageReceived);
}

void erSequencer::setupEcgMode(erNetwork* _network, erMediaPlayer* _player){
    network = _network;
    player = _player;
    ecg.setup(network);
}

void erSequencer::update(ofEventArgs& updateArgs){
    attemptToLoadMediaQueues();
    attemptToLoadCollections();
    if(network->isRunningServer() && ofGetFrameNum() % ER_VIDEO_LENGTH == 0){
        playNewVideo();
    }
    if(network->isRunningServer() && (ofGetFrameNum() % ER_THEME_LENGTH == 0 || currentCollection == "")){
        chooseNewTheme();
    }
}

void erSequencer::messageReceived(string& message){
    erLog("erSequencer::messageReceived(string& message)", message);
    params = translater->toParams(message);
    if(network->isRunningClient() && params.isPlayable()){
        player->playClient(params);
    }
}

string erSequencer::getCurrentCollection(){
    return currentCollection;
}

void erSequencer::playNewVideo(){
    if(queuesLoaded && currentCollection.length() > 0){
        for(auto& queue : queues){
            queue.second.ensureLoaded();
        }
        if(!player->isChannelPlaying(currentChannel)){
            params.newVideoCommand();
            params.setPath(isAudioPlaying() ? queues[currentCollection].getNextSilent() : queues[currentCollection].getNextAudible());
            params.setSpeed(1);
            network->target(currentChannel, params);
            player->playServer(currentChannel, params);
            erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
        }
        incrementCurrentChannel();
    }
}

void erSequencer::chooseNewTheme(){
    if(collectionsLoaded){
        if(currentCollectionIndex < 0 || currentCollectionIndex >= loader->videoCollections.size()){
            currentCollectionIndex = 0;
            random_shuffle(shuffledCollectionIndices.begin(), shuffledCollectionIndices.end());
        }
        currentCollection = loader->videoCollections.at(shuffledCollectionIndices.at(currentCollectionIndex++));
    }
}

void erSequencer::attemptToLoadCollections(){
    if(loader->isLoaded() && shuffledCollectionIndices.size() == 0){
        for(int i = 0; i < loader->videoCollections.size(); i++){
            shuffledCollectionIndices.push_back(i);
        }
        collectionsLoaded = true;
    }
}

void erSequencer::attemptToLoadMediaQueues(){
    if(!queuesLoaded && loader->isLoaded()){
        for(auto collection : loader->videoCollections){
            queues[collection] = queue;
            queues[collection].setup(loader, collection);
        }
        queuesLoaded = true;
    }
}

bool erSequencer::isAudioPlaying(){
    for(auto const& path : loader->audibleVideos){
        if(loader->videoPlayers[path]->isOrWillBePlaying()){
            return true;
        }
    }
    return false;
}

int erSequencer::incrementCurrentChannel(){
    currentChannel++;
    if(currentChannel > settings.numChannels){
        currentChannel = 1;
    }
    return currentChannel;
}