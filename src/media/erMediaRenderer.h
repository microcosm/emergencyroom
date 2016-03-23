#pragma once
#include "ofMain.h"
#include "erMedia.h"
#include "erNetwork.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#define SCREEN_MARGIN 10
#define DOUBLE_SCREEN_MARGIN 20

class erMediaRenderer{

public:
    void setup(erNetwork* _network, int _numChannels);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);

protected:
    void calculatePreviewSize();
    void drawClient();
    void drawServer();
    void drawTestVideoPlayer(int x, int y, int width, int height);
    void drawPreviewBorder(int xi, int yi);
    erNetwork* network;
    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;

    int numChannels, previewBorderWidth, previewBorderHeight, x, y, currentChannel;
    string currentChannelStr;
};