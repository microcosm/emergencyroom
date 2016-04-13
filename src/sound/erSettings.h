#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"

class erSettings{

public:
    void load();

protected:
    string getComputerName();
    FILE *fp;
    int status;
    char path[1];
};