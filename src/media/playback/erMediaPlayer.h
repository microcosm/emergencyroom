#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"
#include "erChannelRenderer.h"
#include "erNetwork.h"

class erMediaPlayer{

public:
    void setup(erNetwork* network);
    void play(erPlayParams params, bool glitch=true);
    void preview(int channel, erPlayParams params);
    bool isChannelPlaying(int channel);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);

protected:
    void playWithGlitch(erPlayParams params);
    void playWithoutGlitch(erPlayParams params);

    erChannelRenderer channelRenderer;
    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    ofPtr<erSyncedVideoPlayer> videoPlayer;
    unsigned long long from;
    float duration;
};