#pragma once
#include "ofMain.h"
#include "erMediaLoader.h"
#include "erMediaRenderer.h"
#include "erNetwork.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"

#define SCREEN_MARGIN 10
#define DOUBLE_SCREEN_MARGIN 20

class erMediaPlayer{

public:
    void setup(erNetwork* _network, int _numChannels);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void play(erPlayParams params);
    void preview(int channel, erPlayParams params);
    bool isChannelPlaying(int channel);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);

protected:
    void calculatePreviewSize();
    bool hasChannel(int channel);
    void drawClient();
    void drawServer();
    void drawPreviewBorder(int xi, int yi, int currentChannel);
    int getX(int xi);
    int getY(int yi);

    erMediaRenderer renderer;
    erNetwork* network;
    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    map<int, ofPtr<erSyncedVideoPlayer>> channelsToPlayers;
    vector<int> toErase;

    int numChannels, previewWidth, previewHeight, x, y, currentChannel;
    string currentChannelStr;
};