#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaManager.h"
#include "erPlayParams.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void messageReceived(string& messageStr);
    void keyReleased(int key);

protected:
    erNetwork network;
    erTranslater* translater;
    erPlayParams params;
    erMediaManager mediaManager;
    int delay;
    float speed;
};