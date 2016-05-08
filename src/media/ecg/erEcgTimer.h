#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "ofxAnimatableFloat.h"

class erEcgTimer{

public:
    void setup();
    void update();
    void start();
    float getPeriodPosition();
    float getPeriodDuration();
    float getCurrentBpm();
    bool isWithinEcgBeepPeriod();
    bool isWithinEcgBeepPeriod(float position);
    bool isStarted();

protected:
    void setDurationFromBpm();
    bool isPlaying;
    ofxAnimatableFloat currentBpm, currentPeriodPosition;
    float currentPeriodDuration;
    bool started;
};