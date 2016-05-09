#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* _network){
    network = _network;
    channelRenderer.setup(network);
    textRenderer.setup();
    ofAddListener(ofEvents().update, this, &erMediaPlayer::update);
    ofAddListener(ofEvents().draw, this, &erMediaPlayer::draw);
    ofAddListener(ofEvents().keyReleased, this, &erMediaPlayer::keyReleased);
}

void erMediaPlayer::setupEcgMode(erNetwork* _network){
    network = _network;
}

void erMediaPlayer::update(ofEventArgs& args){
    if(network->isRunningServer()){
        soundRenderer.ensureSetup();
    }
}

void erMediaPlayer::draw(ofEventArgs& args){
    ofSetColor(ofColor::white);
    if(network->isRunningServer() && settings.serverDrawingEnabled){
        if(soundRenderer.isSyncing()){
            ofDrawBitmapString("SYNCING...", 130, ofGetHeight() - 208);
        }else if(soundRenderer.hasSynced()){
            ofDrawBitmapString("SYNCED", 130, ofGetHeight() - 208);
        }else{
            ofDrawBitmapString("NOT SYNCED", 130, ofGetHeight() - 208);
        }
    }
}

void erMediaPlayer::keyReleased(ofKeyEventArgs &args){
    if(network->isRunningServer() && args.key == '-'){
        network->syncEcg(ECG_SYNC_DELAY);
        soundRenderer.syncEcg(ECG_SYNC_DELAY);
    }
}

void erMediaPlayer::playClient(erPlayParams params){
    if(params.isVideoCommand()){
        calculateVideoPlaybackVariables(params);

        channelRenderer.newOpeningGlitchPeriod(startOpeningGlitch, openingGlitchDuration);
        channelRenderer.newClosingGlitchPeriod(startClosingGlitch, closingGlitchDuration);

        for(int i = 1; i <= intermediateGlitches.size(); i++){
            channelRenderer.newIntermediateGlitchPeriod(i, intermediateGlitches.at(i-1), ofRandom(300, 1000));
        }

        string decoyPath;
        do{
            decoyPath = allVideoPaths->at(floor(ofRandom(allVideoPaths->size() - 0.0001)));
        }
        while(videoPlayers->at(decoyPath)->isOrWillBePlaying());

        channelRenderer.assignDecoyGlitch(videoPlayers->at(decoyPath));

        if(renderText){
            textRenderer.newOverlayPeriod(startTextOverlay, textOverlayDuration);
            textRenderer.newTextPeriod(startText, textDuration, params);
        }

        videoPlayer->execute(params);
    }
}

void erMediaPlayer::playServer(int channel, erPlayParams params){
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

bool erMediaPlayer::isChannelPlaying(int channel){
    return channelRenderer.isChannelPlaying(channel);
}

void erMediaPlayer::setVideoPaths(vector<string> *_videoPaths){
    allVideoPaths = _videoPaths;
}

void erMediaPlayer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
    channelRenderer.setVideoPlayers(videoPlayers);
}

void erMediaPlayer::setTexts(map<string, vector<string>>* texts){
    textRenderer.setTexts(texts);
}

void erMediaPlayer::useSoundRendererFor(vector<string>& audibleVideos){
    soundRenderer.setupVideo(audibleVideos);

    for(const auto& video : audibleVideos){
        videoPlayers->at(video)->renderSoundWith(&soundRenderer);
    }
}

void erMediaPlayer::calculateVideoPlaybackVariables(erPlayParams params){
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

    if(network->isRunningClient() && videoDuration > 7000){
        startText = currentTime + bufferTime + videoGlitchTime + videoDuration * 0.17;
        textDuration = videoDuration * 0.5;
        startTextOverlay = currentTime + bufferTime;
        textOverlayDuration = videoDuration;
        renderText = true;
    }else{
        renderText = false;
    }
}

void erMediaPlayer::calculateSoundPlaybackVariables(){
    startOpeningGlitch = currentTime + bufferTime;
    openingGlitchDuration = videoGlitchTime;
    closingGlitchDuration = videoGlitchTime;
}

string erMediaPlayer::getClientVideoState(){
    return channelRenderer.getClientVideoState();
}