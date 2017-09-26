#pragma once
#include "ofMain.h"
#include "erMediaRenderer.h"
#include "erNetwork.h"
#include "erSoundPlayer.h"
#include "erVideoPlayer.h"

#define SCREEN_MARGIN 10
#define DOUBLE_SCREEN_MARGIN 20

class erChannelRenderer{

public:
    void setup(erNetwork* _network);
    void update();
    void draw();
    void setVideoPlayers(map<string, ofPtr<erVideoPlayer>>* _videoPlayers);
    void setCurrentPlayerPath(string path);

    void newOpeningGlitchPeriod(u_int64_t from, float duration, int channel=1);
    void newClosingGlitchPeriod(u_int64_t from, float duration, int channel=1);
    void newIntermediateGlitchPeriod(int i, u_int64_t from, float duration, int channel=1);
    void assignDecoyGlitch(ofPtr<erVideoPlayer> _videoPlayer);
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
    map<string, ofPtr<erVideoPlayer>>* videoPlayers;
    erVideoPlayer* videoPlayer;

    map<int, ofPtr<erVideoPlayer>> channelsToPlayers;
    vector<int> toErase;

    int previewWidth, previewHeight, x, y, currentChannel;
    string currentChannelStr, currentPlayerPath;
    bool anyPlayerIsPlaying;
};
