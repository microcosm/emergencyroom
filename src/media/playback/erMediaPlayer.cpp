#include "erMediaPlayer.h"

void erMediaPlayer::setup(erNetwork* network){
    channelRenderer.setup(network);
}

void erMediaPlayer::play(erPlayParams params){
    if(params.isVideoCommand()){
        videoPlayers->at(params.getPath())->execute(params);
    }else if(params.isTestCommand()){
        testSoundPlayer->execute(params);
        testVideoPlayer->execute(params);
    }
}

void erMediaPlayer::preview(int channel, erPlayParams params){
    play(params);
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