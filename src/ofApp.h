#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaPlayer.h"
#include "erLogger.h"
#include "erSettings.h"

#define NUM_CHANNELS 1

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    virtual void draw(ofEventArgs& args);
    void keyReleased(int key);

protected:
    erMediaLoader loader;
    erMediaPlayer player;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
    erSettings settings;
};