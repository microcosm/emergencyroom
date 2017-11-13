#include "erMediaSequencer.h"

void erMediaSequencer::setup(erNetwork* _network, erMediaLoader* _mediaLoader, erMediaController* _mediaController, erEcgTimer* _ecgTimer){
    network = _network;
    mediaLoader = _mediaLoader;
    mediaController = _mediaController;
    ecgTimer = _ecgTimer;

    currentSequencerDelay = -1;
    nextTriggerTime = 0;

    currentCollection = "";
    currentCollectionIndex = -1;
    queuesLoaded = false;
    collectionsLoaded = false;

    focusIndex = ofRandom(0, mediaLoader->focusVideos.size());
    focusVideoPath = "";
    focusTime = false;

    currentChannelIndex = -1;
    translater = network->getTranslater();
    loadChannels();

    ofAddListener(network->clientMessageReceived(), this, &erMediaSequencer::messageReceived);
    ofAddListener(ecgTimer->bpmLooped(), this, &erMediaSequencer::ecgBpmLooped);
}

void erMediaSequencer::update(){
    if(bpmLooped){
        handleBpmLooped();
        bpmLooped = false;
    }
    if(stopAllReceived){
        handleStopAll();
        stopAllReceived = false;
    }
    if(aMessageReceived){
        handleMessageReceived();
        aMessageReceived = false;
    }

    setSequencerDelay();
    attemptToLoadMediaQueues();
    attemptToLoadCollections();

    if(focusTime){
        erVideoPlayer* player = mediaLoader->videoPlayers.at(focusVideoPath).get();
        bool isMovieDone = player->getIsMovieDone();

        if(isMovieDone){
            focusTime = false;
            focusVideoPath = "";
            //ecgTimer->resumeBpmAnimation();
        }
    }

    if(!focusTime){
        if(settings.isServer && ofGetElapsedTimeMillis() > nextTriggerTime){
            setSequencerDelay();
            nextTriggerTime += currentSequencerDelay;
            playNewVideo();
        }
        if(settings.isServer && (ofGetFrameNum() % ER_THEME_LENGTH == 0 || currentCollection == "")){
            chooseNewTheme();
        }
    }
}

void erMediaSequencer::draw(){
    ofSetColor(ofColor::white);
    stringstream status;

    status << "=== Collection ===\n";
    status << "Collection...................." << ofToString(currentCollection) << "\n";
    status << "Collection change countdown..." << ER_THEME_LENGTH - (ofGetFrameNum() % ER_THEME_LENGTH) << "\n";
    status << "\n\n";

    status << "=== Video ===\n";
    status << "Play command delay time......." << ofToString(currentSequencerDelay) << "\n";
    status << "Play command countdown........" << ofToString(nextTriggerTime - ofGetElapsedTimeMillis()) << "\n";
    status << "\n\n";

    status << "=== Channel ===\n";
    status << "Channel targeted.............." << (focusTime ? "ALL" : ofToString(currentChannel)) << "\n";
    status << "Playback mode................." << (focusTime ? "FOCUS: " + focusVideoPath : "INDIVIDUATED") << "\n";
    status << "\n\n";

    status << "=== ECG Arc ===\n";
    status << "Current BPM..................." << ofToString(ecgTimer->getCurrentBpm()) << "\n";
    status << "Transitioning from............" << ofToString(bpmDirection == 1 ? settings.ecgLowestBpm : settings.ecgHighestBpm) << "bpm to " << ofToString(bpmDirection == 1 ? settings.ecgHighestBpm : settings.ecgLowestBpm) << "bpm\n";
    status << "Direction....................." << (bpmDirection == 1 ? "Speeding up" : "Slowing down") << "\n";
    status << "Transition duration..........." << settings.ecgBpmPeriodSecs << " seconds\n";
    status << "Transition progress..........." << ecgTimer->getPercentThroughBpmArc() << "%\n";
    status << "\n\n";

    status << "=== ECG Period ===\n";
    status << "Period duration..............." << ofToString(ecgTimer->getPeriodDuration()) << "\n";
    status << "Progress through period......." << ofToString(ecgTimer->getPeriodPosition()) << "\n";

    ofDrawBitmapString(status.str(), 50, 170);
}

void erMediaSequencer::stopAll(){
    network->clientStopAll();
    mediaController->stopAll();
}

void erMediaSequencer::messageReceived(string& message){
    if(message.substr(0, 8) == "STOP ALL"){
        stopAllReceived = true;
    }else{
        messageContent = message;
        aMessageReceived = true;
    }
}

void erMediaSequencer::handleBpmLooped(){
    focusTime = true;
    stopAll();
    //ecgTimer->pauseBpmAnimation();
    if(focusIndex >= mediaLoader->focusVideos.size()){
        focusIndex = 0;
    }
    focusVideoPath = mediaLoader->focusVideos.at(focusIndex);
    prepareParams(focusVideoPath, 1);
    network->flood(params);
    mediaController->floodServer(params);
    focusIndex++;
}

void erMediaSequencer::handleStopAll(){
    if(settings.isClient){
        mediaController->stopAll();
    }
}

void erMediaSequencer::handleMessageReceived(){
    params = translater->toParams(messageContent);
    if(params.isPlayable()){
        mediaController->playClient(params);
    }
}

void erMediaSequencer::setSequencerDelay(){
    currentSequencerDelay = ofMap(ecgTimer->getCurrentBpm(), settings.ecgLowestBpm, settings.ecgHighestBpm, settings.longestSequenceDelay, settings.shortestSequenceDelay);
}

string erMediaSequencer::getCurrentCollection(){
    return currentCollection;
}

void erMediaSequencer::ecgBpmLooped(ofxAnimatable::AnimationEvent& args){
    bpmDirection = args.direction;

    if(args.direction == 1){
        bpmLooped = true;
    }
}

void erMediaSequencer::playNewVideo(){
    if(queuesLoaded && currentCollection.length() > 0){
        for(auto& queue : queues){
            queue.second.ensureLoaded();
        }
        currentChannel = chooseNewChannel();
        if(!mediaController->isChannelPlaying(currentChannel)){
            prepareParams(isAudioPlaying() ? queues[currentCollection].getNextSilent() : queues[currentCollection].getNextAudible(), 1);
            network->target(currentChannel, params);
            mediaController->playServer(currentChannel, params);
        }
    }
}

void erMediaSequencer::chooseNewTheme(){
    if(collectionsLoaded){
        if(currentCollectionIndex < 0 || currentCollectionIndex >= mediaLoader->videoCollections.size()){
            currentCollectionIndex = 0;
            erShuffle(shuffledCollectionIndices);
        }
        currentCollection = mediaLoader->videoCollections.at(shuffledCollectionIndices.at(currentCollectionIndex++));
    }
}

int erMediaSequencer::chooseNewChannel(){
    if(currentChannelIndex < 0 || currentChannelIndex >= settings.numChannels){
        currentChannelIndex = 0;
        erShuffle(shuffledChannels);
    }
    return shuffledChannels.at(currentChannelIndex++);
}

void erMediaSequencer::loadChannels(){
    for(int i = 1; i <= settings.numChannels; i++){
        shuffledChannels.push_back(i);
    }
}

void erMediaSequencer::attemptToLoadCollections(){
    if(mediaLoader->isLoaded() && shuffledCollectionIndices.size() == 0){
        for(int i = 0; i < mediaLoader->videoCollections.size(); i++){
            shuffledCollectionIndices.push_back(i);
        }
        collectionsLoaded = true;
    }
}

void erMediaSequencer::attemptToLoadMediaQueues(){
    if(!queuesLoaded && mediaLoader->isLoaded()){
        for(auto collection : mediaLoader->videoCollections){
            queues[collection] = queue;
            queues[collection].setup(mediaLoader, collection);
        }
        queuesLoaded = true;
    }
}

bool erMediaSequencer::isAudioPlaying(){
    ofPtr<erVideoPlayer> videoPlayer;
    for(auto const& path : mediaLoader->audibleVideos){
        if(mediaLoader->videoPlayers[path]->isOrWillBePlaying()){
            return true;
        }
    }
    return false;
}

void erMediaSequencer::prepareParams(string path, int speed){
    params.newVideoCommand();
    params.setPath(path);
    params.setSpeed(speed);
}
