#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"

class erSound{

public:
    void setup();
    void ensureSetup();
    void play();

protected:
    ofxAudioUnitManager manager;
    ofxAudioUnitChain chain;
    ofxManagedAudioUnit massive;
    bool isSetup = false;
};