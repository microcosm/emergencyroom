#pragma once
#include "ofMain.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erGlitchTimer.h"
#include "erNetwork.h"
#include "ofxEasyFboGlitch.h"

#define ER_PLAYBACK_STATIC "0. Static Playback"
#define ER_PLAYBACK_NORMAL "1. Normal Playback"
#define ER_PLAYBACK_GLITCH "2. Glitch"
#define ER_PLAYBACK_DECOY "3. Decoy Glitch"

class erMediaRenderer : public erGlitchTimer{

public:
    void setup(erNetwork* _network);
    virtual void update(ofEventArgs& args);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);
    void assignDecoyGlitch(erSyncedVideoPlayer* _videoPlayer);

    void draw(erSyncedVideoPlayer* player, int x, int y, int width, int height, int channel=1);
    void drawStatic(int x, int y, int width, int height);
    void drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    string getPlaybackState();

protected:
    void updateDecoyPlayer();
    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;

    erSyncedVideoPlayer* decoyGlitchPlayer;
    int decoyFramesRemaining, minDecoyFrames, maxDecoyFrames;

    erNetwork* network;
    ofFbo fbo;
    ofxEasyFboGlitch fboGlitch;
    bool bufferEmpty;
    int x, y;
    string playbackState;
};