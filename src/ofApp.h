#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erLogger.h"

#define NUM_CHANNELS 2

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyReleased(int key);

protected:
    erMediaManager mediaManager;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
    bool test = false;
};