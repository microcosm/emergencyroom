#pragma once
#include "ofMain.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "ofxEasyFboGlitch.h"

class erMediaRenderer{

public:
    void setup();
    virtual void update(ofEventArgs& args);

    void setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer);
    void setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer);
    void setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers);
    
    void newOpeningGlitchPeriod(u_int64_t from, float duration);
    void newClosingGlitchPeriod(u_int64_t from, float duration);
    void draw(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawStatic(int x, int y, int width, int height);

protected:
    void drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    bool withinGlitchPeriod();

    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;

    ofFbo fbo;
    ofxEasyFboGlitch fboGlitch;
    u_int64_t openingGlitchStart, openingGlitchEnd;
    u_int64_t closingGlitchStart, closingGlitchEnd;
    u_int64_t currentTime;
    bool withinOpeningGlitchPeriod, withinClosingGlitchPeriod;
    bool bufferEmpty;
    int x, y;
};