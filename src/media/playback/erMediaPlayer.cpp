#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* _network, int numChannels, float masterVolume){
    network = _network;
    channelRenderer.setup(network);
    soundRenderer.setMasterVolume(masterVolume);
    soundRenderer.setNumChannels(numChannels);
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
    if(network->isRunningServer()){
        if(soundRenderer.isSyncing()){
            ofDrawBitmapString("SYNCING...", 130, ofGetHeight() - 170);
        }else if(soundRenderer.hasSyncedBefore()){
            ofDrawBitmapString("SYNCED", 130, ofGetHeight() - 170);
        }else{
            ofDrawBitmapString("NOT SYNCED", 130, ofGetHeight() - 170);
        }
    }
}

void erMediaPlayer::keyReleased(ofKeyEventArgs &args){
    if(network->isRunningServer() && args.key == '-'){
        network->syncEcg(ECG_SYNC_DELAY);
        soundRenderer.syncEcg(ECG_SYNC_DELAY);
    }
}

void erMediaPlayer::play(erPlayParams params, bool isClient){
    if(params.isVideoCommand()){
        isClient ? playWithGlitch(params) : playWithSound(params);
    }else if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaPlayer::serverPlay(int channel, erPlayParams params){
    play(params, false);
    channelRenderer.assign(channel, params);
    soundRenderer.setCurrentChannel(channel);
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

void erMediaPlayer::playWithGlitch(erPlayParams params){
    calculateGlitchPlaybackVariables(params);
    channelRenderer.newOpeningGlitchPeriod(currentTime + halfBufferTime, halfBufferTime + videoGlitchTime);
    channelRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime + halfBufferTime);
    videoPlayer->execute(params);
}

void erMediaPlayer::playWithSound(erPlayParams params){
    calculateGlitchPlaybackVariables(params);
    soundRenderer.newOpeningGlitchPeriod(currentTime + bufferTime, videoGlitchTime);
    soundRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime);
    videoPlayer->execute(params);
}

void erMediaPlayer::calculateGlitchPlaybackVariables(erPlayParams params){
    videoPlayer = videoPlayers->at(params.getPath());
    currentTime = ofGetElapsedTimeMillis();

    bufferTime = params.getDelay();
    halfBufferTime = bufferTime * 0.5;

    videoDuration = videoPlayer->getDuration() * 1000;
    videoGlitchTime = ofClamp(videoDuration * 0.2, 50, halfBufferTime);
}