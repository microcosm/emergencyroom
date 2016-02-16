#pragma once
#include "ofMain.h"
#include "erSequencer.h"

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
};