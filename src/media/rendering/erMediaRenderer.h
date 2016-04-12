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
    
    void newOpeningGlitchPeriod(unsigned long long from, float duration);
    void newClosingGlitchPeriod(unsigned long long from, float duration);
    void draw(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawStatic();

protected:
    void drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    bool withinGlitchPeriod();

    erSyncedSoundPlayer* testSoundPlayer;
    erSyncedVideoPlayer* testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>>* videoPlayers;

    ofFbo fbo;
    ofxEasyFboGlitch fboGlitch;
    int64_t openingGlitchStart, openingGlitchEnd;
    int64_t closingGlitchStart, closingGlitchEnd;
    int64_t currentTime;
    bool withinOpeningGlitchPeriod, withinClosingGlitchPeriod;
};