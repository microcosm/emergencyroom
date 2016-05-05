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
        }else if(soundRenderer.hasSyncedBefore()){
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

void erMediaPlayer::playTest(erPlayParams params){
    if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaPlayer::playClient(erPlayParams params){
    if(params.isVideoCommand()){
        calculateVideoPlaybackVariables(params);

        channelRenderer.newOpeningGlitchPeriod(startOpeningGlitch, openingGlitchDuration);
        channelRenderer.newClosingGlitchPeriod(startClosingGlitch, closingGlitchDuration);

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

void erMediaPlayer::setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer){
    testSoundPlayer = _testSoundPlayer;
    channelRenderer.setTestSoundPlayer(testSoundPlayer);
}

void erMediaPlayer::setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer){
    testVideoPlayer = _testVideoPlayer;
    channelRenderer.setTestVideoPlayer(testVideoPlayer);
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