#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaPlayer.h"
#include "erLogger.h"
#include "erSettings.h"

#define NUM_CHANNELS 3

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw(ofEventArgs& args);
    void keyReleased(int key);

protected:
    erMediaLoader loader;
    erMediaPlayer player;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
};