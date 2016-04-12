#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* network){
    channelRenderer.setup(network);
}

void erMediaPlayer::play(erPlayParams params, bool glitch){
    if(params.isVideoCommand()){
        glitch ? playWithGlitch(params) : playWithoutGlitch(params);
    }else if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaPlayer::preview(int channel, erPlayParams params){
    play(params, false);
    channelRenderer.assign(channel, params);
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
    videoPlayer = videoPlayers->at(params.getPath());
    currentTime = ofGetElapsedTimeMillis();

    bufferTime = params.getDelay();
    halfBufferTime = bufferTime * 0.5;

    videoDuration = videoPlayer->getDuration() * 1000;
    videoGlitchTime = ofClamp(videoDuration * 0.2, 50, halfBufferTime);

    channelRenderer.newOpeningGlitchPeriod(currentTime + halfBufferTime, halfBufferTime + videoGlitchTime);
    channelRenderer.newClosingGlitchPeriod(currentTime + bufferTime + videoDuration - videoGlitchTime + COSMOLOGICAL_CONSTANT, videoGlitchTime + halfBufferTime);

    videoPlayer->execute(params);
}

void erMediaPlayer::playWithoutGlitch(erPlayParams params){
    videoPlayers->at(params.getPath())->execute(params);
}