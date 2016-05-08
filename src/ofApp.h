#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaPlayer.h"
#include "erLogger.h"
#include "erSettings.h"
#define ER_FONT_PATH "font/klima-medium-web.ttf"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw(ofEventArgs& args);
    void drawFps(int x, int y);
    void keyReleased(int key);

protected:
    ofTrueTypeFont bigFont, smallFont;
    erMediaLoader loader;
    erMediaPlayer player;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
    bool videoSoundAssigned;
    int width, height;
};