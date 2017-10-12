#pragma once
#include "ofMain.h"
#include "erLogger.h"
#include "erSettings.h"
#include "erMediaSequencer.h"
#include "erMediaController.h"
#include "erClientController.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawFps(int x, int y);
    void keyReleased(int key);
    void toggleFullscreen();
    void exit();

protected:
    ofTrueTypeFont bigFont, smallFont;
    erMediaLoader mediaLoader;
    erMediaController mediaController;
    erMediaSequencer mediaSequencer;
    erNetwork network;
    erPlayParams params;
    erClientController clientController;
    erEcgTimer ecgTimer;
    bool videoSoundAssigned;
    int width, height;
};
