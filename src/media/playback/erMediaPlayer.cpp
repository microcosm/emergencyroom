#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* _network){
    network = _network;
    channelRenderer.setup(network);
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
        calculateGlitchPlaybackVariables(params);
        channelRenderer.newOpeningGlitchPeriod(currentTime + halfBufferTime, halfBufferTime + videoGlitchTime);
        channelRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime + halfBufferTime);
        videoPlayer->execute(params);
    }
}

void erMediaPlayer::playServer(int channel, erPlayParams params){
    if(params.isVideoCommand()){
        calculateGlitchPlaybackVariables(params);
        channelRenderer.assign(channel, params);
        channelRenderer.newOpeningGlitchPeriod(currentTime + halfBufferTime, halfBufferTime + videoGlitchTime, channel);
        channelRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime + halfBufferTime, channel);
        soundRenderer.newOpeningGlitchPeriod(currentTime + bufferTime, videoGlitchTime, channel);
        soundRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime, channel);
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

void erMediaPlayer::calculateGlitchPlaybackVariables(erPlayParams params){
    videoPlayer = videoPlayers->at(params.getPath());
    currentTime = ofGetElapsedTimeMillis();

    bufferTime = params.getDelay();
    halfBufferTime = bufferTime * 0.5;

    videoDuration = videoPlayer->getDuration() * 1000;
    videoGlitchTime = ofClamp(videoDuration * 0.2, 50, halfBufferTime);
}