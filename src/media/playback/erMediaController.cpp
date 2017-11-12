#include "erMediaController.h"

void erMediaController::setup(erNetwork* _network, erEcgTimer* ecgTimer, vector<string>* _videoPaths, map<string, ofPtr<erVideoPlayer>>* _videoPlayers, erOmxManager* omxManager){
    network = _network;
    setVideoPaths(_videoPaths);
    setVideoPlayers(_videoPlayers);

    channelRenderer.setup(network, omxManager);
    if(settings.isServer){
        soundRenderer.setup(ecgTimer);
        ecgRenderer.setup(ecgTimer);
    }
}

void erMediaController::update(){
    channelRenderer.update();
    if(settings.isServer){
        soundRenderer.update();
        ecgRenderer.update();
    }
}

void erMediaController::draw(){
    ofSetColor(ofColor::white);
    channelRenderer.draw();
    if(settings.isServer){
        soundRenderer.draw();
        ecgRenderer.draw();
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

        //decoyPath = selectDecoyPath(params);
        channelRenderer.setCurrentPlayerPath(params.getPath());
        //channelRenderer.assignDecoyGlitch(videoPlayers->at(decoyPath));

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
}

void erMediaController::calculateSoundPlaybackVariables(){
    startOpeningGlitch = currentTime + bufferTime;
    openingGlitchDuration = videoGlitchTime;
    closingGlitchDuration = videoGlitchTime;
}

string erMediaController::getClientVideoState(){
    return channelRenderer.getClientVideoState();
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
