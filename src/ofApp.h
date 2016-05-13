#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaPlayer.h"
#include "erLogger.h"
#include "erSettings.h"
#include "erEcgRenderer.h"
#define ER_FONT_PATH "font/klima-medium-web.ttf"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawFps(int x, int y);
    void keyReleased(int key);

protected:
    ofTrueTypeFont bigFont, smallFont;
    erMediaLoader loader;
    erMediaPlayer player;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
    erEcgRenderer ecg;
    bool videoSoundAssigned;
    int width, height;
};