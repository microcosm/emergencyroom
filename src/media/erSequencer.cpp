#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player){
    network = _network;
    loader = _loader;
    player = _player;

    shuffledIndexingSetup = false;
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

void erSequencer::setupShuffledIndexing(){
    currentAudibleVideoIndex = -1;
    currentSilentVideoIndex = -1;

    shuffledAudibleVideoIndices.clear();
    for(int i = 0; i < loader->audibleVideos.size(); i++){
        shuffledAudibleVideoIndices.push_back(i);
    }

    shuffledSilentVideoIndices.clear();
    for(int i = 0; i < loader->silentVideos.size(); i++){
        shuffledSilentVideoIndices.push_back(i);
    }

    shuffledIndexingSetup = true;
}

void erSequencer::update(ofEventArgs& updateArgs){
    if(network->isRunningServer() && ofGetFrameNum() % ER_VIDEO_LENGTH == 0){
        playNewVideo();
    }
}

void erSequencer::messageReceived(string& message){
    erLog("erSequencer::messageReceived(string& message)", message);
    params = translater->toParams(message);
    if(network->isRunningClient() && params.isPlayable()){
        player->playClient(params);
    }
}

void erSequencer::playNewVideo(){
    if(loader->isLoaded()){
        if(!shuffledIndexingSetup){
            setupShuffledIndexing();
        }
        if(!player->isChannelPlaying(currentChannel)){
            params.newVideoCommand();
            params.setPath(isAudioPlaying() ? getNextSilent() : getNextAudible());
            params.setSpeed(1);
            network->target(currentChannel, params);
            player->playServer(currentChannel, params);
            erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
        }
        incrementCurrentChannel();
    }
}

string erSequencer::getNextSilent(){
    if(currentSilentVideoIndex < 0 || currentSilentVideoIndex >= loader->silentVideos.size()){
        currentSilentVideoIndex = 0;
        random_shuffle(shuffledSilentVideoIndices.begin(), shuffledSilentVideoIndices.end());
    }
    return loader->silentVideos.at(shuffledSilentVideoIndices.at(currentSilentVideoIndex++));
}

string erSequencer::getNextAudible(){
    if(currentAudibleVideoIndex < 0 || currentAudibleVideoIndex >= loader->audibleVideos.size()){
        currentAudibleVideoIndex = 0;
        random_shuffle(shuffledAudibleVideoIndices.begin(), shuffledAudibleVideoIndices.end());
    }
    return loader->audibleVideos.at(shuffledAudibleVideoIndices.at(currentAudibleVideoIndex++));
}

string erSequencer::chooseVideo(){
    string path;
    do{
        path = chooseRandom(isAudioPlaying() ? &loader->silentVideos : &loader->audibleVideos);
    }while(loader->videoPlayers[path]->isOrWillBePlaying());
    return path;
}

string erSequencer::chooseRandom(vector<string>* videos){
    int index = (int)floor(ofRandom(videos->size()));
    index = index == videos->size() ? index - 1 : index;
    return videos->at(index);
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