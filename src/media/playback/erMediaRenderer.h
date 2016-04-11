#pragma once
#include "ofMain.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "ofxEasyFboGlitch.h"

class erMediaRenderer{

public:
    void drawVideo(erSyncedVideoPlayer* player, int x, int y, int width, int height);
    void drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height);

protected:

    ofFbo fbo;
    ofxEasyFboGlitch fboGlitch;
};