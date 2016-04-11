#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"
#include "erChannelRenderer.h"
#include "erNetwork.h"

class erMediaPlayer{

public:
    void setup(erNetwork* network);
    void play(erPlayParams params);
    void preview(int channel, erPlayParams params);
    bool isChannelPlaying(int channel);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);

protected:
    erChannelRenderer channelRenderer;
    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
};