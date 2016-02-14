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

protected:
    erNetwork network;
    erSyncedSoundPlayer soundPlayer;
    erSyncedVideoPlayer videoPlayer;
    int delay;
    float speed;
    vector<string> messageParts, argumentParts, variableParts;
};
