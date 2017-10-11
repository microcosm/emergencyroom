#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "ofxAnimatableFloat.h"

class erEcgTimer{

public:
    void setup();
    void update();
    float getPeriodPosition();
    float getPeriodDuration();
    float getCurrentBpm();
    bool isWithinEcgBeepPeriod();
    bool isWithinEcgBeepPeriod(float position);
    ofEvent<ofxAnimatable::AnimationEvent>& bpmLooped();
    void pauseBpmAnimation();
    void resumeBpmAnimation();

protected:
    void setDurationFromBpm();
    bool isPlaying;
    ofxAnimatableFloat currentBpm, currentPeriodPosition;
    float currentPeriodDuration;
};
