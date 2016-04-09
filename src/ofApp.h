#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaRenderer.h"
#include "erLogger.h"

#define NUM_CHANNELS 2

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    virtual void draw(ofEventArgs& args);
    void keyReleased(int key);

protected:
    erMediaLoader mediaLoader;
    erMediaRenderer renderer;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
    bool test = false;
};