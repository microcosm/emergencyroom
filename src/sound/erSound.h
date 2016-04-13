#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"

class erSound{

public:
    void setup();
    void play();

protected:
    ofxAudioUnitManager manager;
    ofxAudioUnitChain chain;
    ofxManagedAudioUnit massive;
};