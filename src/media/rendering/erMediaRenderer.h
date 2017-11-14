#pragma once
#include "ofMain.h"
#include "erSoundPlayer.h"
#include "erOmxManager.h"
#include "erVideoPlayer.h"
#include "erGlitchTimer.h"
#include "erNetwork.h"
#include "ofxEasyFboGlitch.h"

#define ER_PLAYBACK_STATIC "0. Static Playback"
#define ER_PLAYBACK_NORMAL "1. Normal Playback"
#define ER_PLAYBACK_GLITCH "2. Glitch"
#define ER_PLAYBACK_DECOY "3. Decoy Glitch"

class erMediaRenderer : public erGlitchTimer{

public:
    void setup(erNetwork* _network, erOmxManager* _omxManager);
    void update();

    void setVideoPlayers(map<string, ofPtr<erVideoPlayer>>* _videoPlayers);
    void assignDecoyGlitch(erVideoPlayer* _videoPlayer);

    void drawServer(erVideoPlayer* player, int x, int y, int width, int height, int channel=1);
    void drawClient(int x, int y, int width, int height, int channel=1);
    void drawStatic(int x, int y, int width, int height);
    void drawNormal(erVideoPlayer* player, int x, int y, int width, int height);
    void drawGlitched(erVideoPlayer* player, int x, int y, int width, int height);
    void drawClientNormal(int x, int y, int width, int height);
    void drawClientGlitched(int x, int y, int width, int height);
    string getPlaybackState();
    void stopDecoyPlayer();

protected:
    map<string, ofPtr<erVideoPlayer>>* videoPlayers;

//    erSyncedVideoPlayer* decoyGlitchPlayer;
//    int decoyFramesRemaining, minDecoyFrames, maxDecoyFrames;

    erNetwork* network;
    erOmxManager* omxManager;
    ofFbo fbo;
    ofxEasyFboGlitch fboGlitch;
    bool bufferEmpty, decoyAssigned;
    int x, y, fboWidth, fboHeight;
    string playbackState;
};
