#include "erEcgTimer.h"

void erEcgTimer::setup(){
    started = false;

    currentBpm.setRepeatType(LOOP_BACK_AND_FORTH);
    currentBpm.setCurve(EASE_IN_EASE_OUT);
    currentBpm.setDuration(settings.ecgBpmPeriodSecs);

    currentPeriodPosition.setRepeatType(LOOP);
    currentPeriodPosition.setCurve(LINEAR);
}

void erEcgTimer::update(){
    currentBpm.update(ofGetLastFrameTime());
    currentPeriodPosition.update(ofGetLastFrameTime());

    setDurationFromBpm();
    currentPeriodPosition.setDuration(currentPeriodDuration * 0.001);
}

void erEcgTimer::start(){
    started = true;
    currentBpm.animateFromTo(settings.ecgLowestBpm, settings.ecgHighestBpm);
    currentPeriodPosition.animateFromTo(0, 1);
}

float erEcgTimer::getPeriodPosition(){
    return currentPeriodPosition.val();
}

float erEcgTimer::getPeriodDuration(){
    return currentPeriodDuration;
}

float erEcgTimer::getCurrentBpm(){
    return currentBpm.val();
}

bool erEcgTimer::isWithinEcgBeepPeriod(){
    return isWithinEcgBeepPeriod(currentPeriodPosition.val());
}

bool erEcgTimer::isWithinEcgBeepPeriod(float position){
    if(started){
        return position > settings.ecgBeginBeepAt && position < settings.ecgEndBeepAt;
    }
    return false;
}

bool erEcgTimer::isStarted(){
    return started;
}

ofEvent<ofxAnimatable::AnimationEvent>& erEcgTimer::bpmLooped(){
    return currentBpm.animLooped;
}

void erEcgTimer::setDurationFromBpm(){
    currentPeriodDuration = 60000 / currentBpm.val();
}