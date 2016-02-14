#pragma once

#include "ofMain.h"
#include "erNetwork.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erSyncedHapPlayer.h"

#define DEFAULT_DELAY 3000
#define TEST_COMMAND "TEST"
#define PLAY_COMMAND "PLAY"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void play(int delay, float speed);
    void messageReceived(string & message);
    void keyReleased(int key);

protected:
    erNetwork network;
    erSyncedSoundPlayer soundPlayer;
    erSyncedVideoPlayer videoPlayer;
    int delay;
    float speed;
    vector<string> messageParts, argumentParts, variableParts;
};
