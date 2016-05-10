#include "erSequencer.h"

void erSequencer::setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player){
    network = _network;
    loader = _loader;
    player = _player;
    ecgTimer = player->getEcgTimer();
    ecgTimerStarted = false;
    currentSequencerDelay = -1;
    nextTriggerTime = 0;

    currentCollection = "";
    currentCollectionIndex = -1;
    queuesLoaded = false;
    collectionsLoaded = false;

    focusIndex = 0;
    focusVideoPath = "";
    focusTime = false;

    currentChannelIndex = -1;
    translater = network->getTranslater();
    loadChannels();

    ofAddListener(ofEvents().update, this, &erSequencer::update);
    ofAddListener(ofEvents().draw, this, &erSequencer::draw);
    ofAddListener(network->clientMessageReceived(), this, &erSequencer::messageReceived);
    ofAddListener(ecgTimer->bpmLooped(), this, &erSequencer::ecgBpmLooped);
}

void erSequencer::setupEcgMode(erNetwork* _network, erMediaPlayer* _player){
    network = _network;
    player = _player;
    ecg.setup(network);
}

void erSequencer::update(ofEventArgs& args){
    ecgTimerStarted = ecgTimer != NULL && ecgTimer->isStarted();
    setSequencerDelay();
    attemptToLoadMediaQueues();
    attemptToLoadCollections();

    if(focusTime){
        erSyncedVideoPlayer* player = loader->videoPlayers.at(focusVideoPath).get();
        player->lock();
        bool isMovieDone = player->getIsMovieDone();
        player->unlock();

        if(isMovieDone){
            focusTime = false;
            focusVideoPath = "";
            ecgTimer->resumeBpmAnimation();
        }
    }

    if(!focusTime){
        if(network->isRunningServer() && ofGetElapsedTimeMillis() > nextTriggerTime && currentSequencerDelay != -1){
            nextTriggerTime += currentSequencerDelay;
            playNewVideo();
        }
        if(network->isRunningServer() && (ofGetFrameNum() % ER_THEME_LENGTH == 0 || currentCollection == "")){
            chooseNewTheme();
        }
    }
}

void erSequencer::draw(ofEventArgs& args){
    if(ecgTimerStarted){
        ofDrawBitmapString("Progress:         " + ofToString(ecgTimer->getPeriodPosition()), ofGetWidth() - 260, ofGetHeight() - 360);
        ofDrawBitmapString("Current duration: " + ofToString(ecgTimer->getPeriodDuration()), ofGetWidth() - 260, ofGetHeight() - 330);
        ofDrawBitmapString("Current ECG BPM:  " + ofToString(ecgTimer->getCurrentBpm()), ofGetWidth() - 260, ofGetHeight() - 300);
        ofDrawBitmapString("Current delay:    " + ofToString(currentSequencerDelay), ofGetWidth() - 260, ofGetHeight() - 270);
    }
}

void erSequencer::stopAll(){
    network->clientStopAll();
    player->stopAll();
}

void erSequencer::messageReceived(string& message){
    erLog("erSequencer::messageReceived(string& message)", message);
    if(network->isRunningClient()){
        if(message.substr(0, 8) == "STOP ALL"){
            player->stopAll();
        }else{
            params = translater->toParams(message);
            if(params.isPlayable()){
                player->playClient(params);
            }
        }
    }
}

void erSequencer::setSequencerDelay(){
    if(ecgTimerStarted){
        currentSequencerDelay = ofMap(ecgTimer->getCurrentBpm(), settings.ecgLowestBpm, settings.ecgHighestBpm, settings.longestSequenceDelay, settings.shortestSequenceDelay);
    }
}

string erSequencer::getCurrentCollection(){
    return currentCollection;
}

void erSequencer::ecgBpmLooped(ofxAnimatable::AnimationEvent& args){
    if(args.direction == 1){
        focusTime = true;
        stopAll();
        ecgTimer->pauseBpmAnimation();
        if(focusIndex >= loader->focusVideos.size()){
            focusIndex = 0;
        }
        focusVideoPath = loader->focusVideos.at(focusIndex);
        prepareParams(focusVideoPath, 1);
        network->flood(params);
        player->floodServer(params);
        focusIndex++;
    }
}

void erSequencer::playNewVideo(){
    if(queuesLoaded && currentCollection.length() > 0){
        for(auto& queue : queues){
            queue.second.ensureLoaded();
        }
        currentChannel = chooseNewChannel();
        if(!player->isChannelPlaying(currentChannel)){
            prepareParams(isAudioPlaying() ? queues[currentCollection].getNextSilent() : queues[currentCollection].getNextAudible(), 1);
            network->target(currentChannel, params);
            player->playServer(currentChannel, params);
            erLog("erSequencer::playNewVideo()", "Target channel " + ofToString(currentChannel) + " " + params.getArgumentStr());
        }
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

int erSequencer::chooseNewChannel(){
    if(currentChannelIndex < 0 || currentChannelIndex >= settings.numChannels){
        currentChannelIndex = 0;
        random_shuffle(shuffledChannels.begin(), shuffledChannels.end());
    }
    return shuffledChannels.at(currentChannelIndex++);
}

void erSequencer::loadChannels(){
    for(int i = 1; i <= settings.numChannels; i++){
        shuffledChannels.push_back(i);
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
    ofPtr<erSyncedVideoPlayer> videoPlayer;
    for(auto const& path : loader->audibleVideos){
        videoPlayer = loader->videoPlayers[path];
        videoPlayer->lock();
        bool playing = loader->videoPlayers[path]->isOrWillBePlaying();
        videoPlayer->unlock();
        if(playing){
            return true;
        }
    }
    return false;
}

void erSequencer::prepareParams(string path, int speed){
    params.newVideoCommand();
    params.setPath(path);
    params.setSpeed(speed);
}