#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"
#include "erChannelRenderer.h"
#include "erSound.h"
#include "erNetwork.h"

#define COSMOLOGICAL_CONSTANT 500

class erMediaPlayer{

public:
    void setup(erNetwork* _network);
    void update(ofEventArgs& args);
    void play(erPlayParams params, bool isClient=true);
    void serverPlay(int channel, erPlayParams params);
    bool isChannelPlaying(int channel);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);

protected:
    void playWithGlitch(erPlayParams params);
    void playWithoutGlitch(erPlayParams params);

    erChannelRenderer channelRenderer;
    erSound sound;
    erNetwork* network;

    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    ofPtr<erSyncedVideoPlayer> videoPlayer;

    unsigned long long currentTime;
    float videoDuration, videoGlitchTime, bufferTime, halfBufferTime;
};