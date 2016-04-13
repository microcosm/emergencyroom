#pragma once
#include "ofMain.h"
#include "erSequencer.h"
#include "erMediaPlayer.h"
#include "erSound.h"
#include "erLogger.h"

#define NUM_CHANNELS 9

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    virtual void draw(ofEventArgs& args);
    void keyReleased(int key);

protected:
    erMediaLoader loader;
    erMediaPlayer player;
    erSound sound;
    erNetwork network;
    erSequencer sequencer;
    erPlayParams params;
};