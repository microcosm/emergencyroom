#pragma once

#include "ofMain.h"
#include "erNetwork.h"

#define SOUND_PLAYER_DELAY 1000
#define TEST_COMMAND "TEST"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void onMessageReceived(string & message);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    erNetwork network;
    ofSoundPlayerDelayed player;
    int time;
    vector<string> parts;
};
