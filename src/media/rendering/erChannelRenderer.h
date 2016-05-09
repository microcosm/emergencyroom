#pragma once
#include "ofMain.h"
#include "erMediaRenderer.h"
#include "erNetwork.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"

#define SCREEN_MARGIN 10
#define DOUBLE_SCREEN_MARGIN 20

class erChannelRenderer{

public:
    void setup(erNetwork* _network);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);
    void setCurrentPlayerPath(string path);

    void newOpeningGlitchPeriod(u_int64_t from, float duration, int channel=1);
    void newClosingGlitchPeriod(u_int64_t from, float duration, int channel=1);
    void newIntermediateGlitchPeriod(int i, u_int64_t from, float duration, int channel=1);
    void assignDecoyGlitch(ofPtr<erSyncedVideoPlayer> _videoPlayer);
    bool isChannelPlaying(int channel);
    void assign(int channel, erPlayParams params);
    string getClientVideoState();

protected:
    void calculatePreviewSize();
    bool hasChannel(int channel);
    void eraseCompletedVideosFromChannels();
    void drawClient();
    void drawServer();
    void drawPreviewBorder(int xi, int yi, int currentChannel);
    int getX(int xi);
    int getY(int yi);

    erMediaRenderer mediaRenderer;
    erNetwork* network;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;
    erSyncedVideoPlayer* videoPlayer;

    map<int, ofPtr<erSyncedVideoPlayer>> channelsToPlayers;
    vector<int> toErase;

    int previewWidth, previewHeight, x, y, currentChannel;
    string currentChannelStr, currentPlayerPath;
    bool anyPlayerIsPlaying;
};