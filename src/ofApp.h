#pragma once
#include "ofMain.h"
#include "erMediaSequencer.h"
#include "erMediaController.h"
#include "erLogger.h"
#include "erSettings.h"
#include "erEcgRenderer.h"
#include "erClientController.h"
#define ER_FONT_PATH "font/klima-medium-web.ttf"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawFps(int x, int y);
    void keyReleased(int key);
    void toggleFullscreen();

protected:
    ofTrueTypeFont bigFont, smallFont;
    erMediaLoader mediaLoader;
    erMediaController mediaController;
    erMediaSequencer mediaSequencer;
    erNetwork network;
    erPlayParams params;
    erEcgRenderer ecg;
    erClientController clientController;
    bool videoSoundAssigned;
    int width, height;
};
