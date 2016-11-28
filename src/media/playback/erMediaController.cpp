#include "erMediaController.h"

void erMediaController::setup(erNetwork* _network){
    network = _network;
    channelRenderer.setup(network);
    //textRenderer.setup();
    ofAddListener(ofEvents().keyReleased, this, &erMediaController::keyReleased);
}

void erMediaController::setupEcgMode(erNetwork* _network){
    network = _network;
}

void erMediaController::update(){
    if(settings.isServer){
        if(syncCommandReceived){
            network->syncEcg(ECG_SYNC_DELAY);
            soundRenderer.syncEcg(ECG_SYNC_DELAY);
            syncCommandReceived = false;
        }
        soundRenderer.ensureSetup();
    }
    channelRenderer.update();
    //textRenderer.update();
    if(soundRenderer.isSetup()){
        soundRenderer.update();
    }
}

void erMediaController::draw(){
    ofSetColor(ofColor::white);
    if(settings.isServer && settings.serverDrawingEnabled){
        if(soundRenderer.isSyncing()){
            ofDrawBitmapString("SYNCING...", 130, ofGetHeight() - 208);
        }else if(soundRenderer.hasSynced()){
            ofDrawBitmapString("SYNCED", 130, ofGetHeight() - 208);
        }else{
            ofDrawBitmapString("NOT SYNCED", 130, ofGetHeight() - 208);
        }
    }
    channelRenderer.draw();
    //textRenderer.draw();
    if(soundRenderer.isSetup()){
        soundRenderer.draw();
    }
}

void erMediaController::keyReleased(ofKeyEventArgs &args){
    if(args.key == '-'){
        syncCommandReceived = true;
    }
}

void erMediaController::playClient(erPlayParams params){
    if(params.isVideoCommand()){
        calculateVideoPlaybackVariables(params);

        channelRenderer.newOpeningGlitchPeriod(startOpeningGlitch, openingGlitchDuration);
        channelRenderer.newClosingGlitchPeriod(startClosingGlitch, closingGlitchDuration);

        for(int i = 1; i <= intermediateGlitches.size(); i++){
            channelRenderer.newIntermediateGlitchPeriod(i, intermediateGlitches.at(i-1), ofRandom(300, 1000));
        }

        decoyPath = selectDecoyPath(params);
        channelRenderer.setCurrentPlayerPath(params.getPath());
        channelRenderer.assignDecoyGlitch(videoPlayers->at(decoyPath));

        if(renderText){
            textRenderer.newOverlayPeriod(startTextOverlay, textOverlayDuration);
            textRenderer.newTextPeriod(startText, textDuration, params);
        }
        videoPlayer->execute(params);
    }
}

void erMediaController::playServer(int channel, erPlayParams params){
    if(params.isVideoCommand()){
        calculateVideoPlaybackVariables(params);
        channelRenderer.assign(channel, params);

        channelRenderer.newOpeningGlitchPeriod(startOpeningGlitch, openingGlitchDuration, channel);
        channelRenderer.newClosingGlitchPeriod(startClosingGlitch, closingGlitchDuration, channel);

        calculateSoundPlaybackVariables();
        soundRenderer.newOpeningGlitchPeriod(startOpeningGlitch, openingGlitchDuration, channel);
        soundRenderer.newClosingGlitchPeriod(startClosingGlitch, closingGlitchDuration, channel);

        videoPlayer->execute(params);
    }
}

void erMediaController::floodServer(erPlayParams params){
    for(int i = 1; i <= settings.numChannels; i++){
        playServer(i, params);
    }
}

void erMediaController::stopAll(){
    for(const auto& videoPlayer : *videoPlayers){
        videoPlayer.second->stop();
    }
    soundRenderer.stopVideoSound();
}

bool erMediaController::isChannelPlaying(int channel){
    return channelRenderer.isChannelPlaying(channel);
}

void erMediaController::setVideoPaths(vector<string> *_videoPaths){
    allVideoPaths = _videoPaths;
}

void erMediaController::setVideoPlayers(map<string, ofPtr<erVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
    channelRenderer.setVideoPlayers(videoPlayers);
}

void erMediaController::setTexts(map<string, vector<string>>* texts){
    textRenderer.setTexts(texts);
}

void erMediaController::useSoundRendererFor(vector<string>& audibleVideos){
    soundRenderer.setupVideo(audibleVideos);

    for(const auto& video : audibleVideos){
        videoPlayers->at(video)->renderSoundWith(&soundRenderer);
    }
}

void erMediaController::calculateVideoPlaybackVariables(erPlayParams params){
    videoPlayer = videoPlayers->at(params.getPath());
    currentTime = ofGetElapsedTimeMillis();

    bufferTime = params.getDelay();
    halfBufferTime = bufferTime * 0.5;

    videoDuration = videoPlayer->getDuration() * 1000;
    videoGlitchTime = ofClamp(videoDuration * 0.2, 50, halfBufferTime);

    startOpeningGlitch = currentTime + halfBufferTime;
    openingGlitchDuration = halfBufferTime + videoGlitchTime;

    startClosingGlitch = currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT;
    closingGlitchDuration = videoGlitchTime + halfBufferTime;

    intermediateGlitches.clear();
    intermediateGlitchDurations.clear();
    for(int i = 0; i < 3; i++){
        intermediateGlitches.push_back(currentTime + bufferTime + ofRandom(videoDuration));
        intermediateGlitchDurations.push_back(ofClamp(ofRandom(videoDuration * 0.03, videoDuration * 0.5), 50, 2000));
    }

    if(settings.isClient && videoDuration > 7000){
        startText = currentTime + bufferTime + videoGlitchTime + videoDuration * 0.17;
        textDuration = videoDuration * 0.5;
        startTextOverlay = currentTime + bufferTime;
        textOverlayDuration = videoDuration;
        renderText = true;
    }else{
        renderText = false;
    }
}

void erMediaController::calculateSoundPlaybackVariables(){
    startOpeningGlitch = currentTime + bufferTime;
    openingGlitchDuration = videoGlitchTime;
    closingGlitchDuration = videoGlitchTime;
}

string erMediaController::getClientVideoState(){
    return channelRenderer.getClientVideoState();
}

erEcgTimer* erMediaController::getEcgTimer(){
    return soundRenderer.getEcgTimer();
}

string erMediaController::selectDecoyPath(erPlayParams params){
    bool found = false;
    do{
        int decoyIndex = floor(ofRandom(allVideoPaths->size() - 0.0001));
        decoyPath = allVideoPaths->at(decoyIndex);
        if(decoyPath != params.getPath()){//? string comparison ?
            if(!videoPlayers->at(decoyPath)->isOrWillBePlaying()){
                found = true;
            }
        }
    }
    while(!found);
    return decoyPath;
}
