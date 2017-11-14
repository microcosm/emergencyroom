#ifdef __linux__
#pragma once
#include "ofMain.h"
#include "ofxOMXPlayer.h"

class erOmxPlayer : public ofxOMXPlayer{

public:
    void prepareToPlay(string absolutePath);
    void startPlaying();
    void update();
    bool isActuallyPlaying();
protected:
    bool isPlaying = false;
};
#endif
