#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erPlayParams.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erSyncedHapPlayer.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void play();
    void messageReceived(string& messageStr);
    void keyReleased(int key);

protected:
    erNetwork network;
    erTranslater* translater;
    erSyncedSoundPlayer soundPlayer;
    erSyncedVideoPlayer videoPlayer;
    erPlayParams params;
    int delay;
    float speed;
};