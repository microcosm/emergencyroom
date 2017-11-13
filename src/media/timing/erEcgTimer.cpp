#include "erEcgTimer.h"

void erEcgTimer::setup(){
    currentBpm.setRepeatType(LOOP_BACK_AND_FORTH);
    currentBpm.setCurve(EASE_IN_EASE_OUT);
    currentBpm.setDuration(settings.ecgBpmPeriodSecs);

    currentPeriodPosition.setRepeatType(LOOP);
    currentPeriodPosition.setCurve(LINEAR);

    currentBpm.animateFromTo(settings.ecgLowestBpm, settings.ecgHighestBpm);
    currentPeriodPosition.animateFromTo(0, 1);
}

void erEcgTimer::update(){
    currentBpm.update(ofGetLastFrameTime());
    currentPeriodPosition.update(ofGetLastFrameTime());

    setDurationFromBpm();
    currentPeriodPosition.setDuration(getPeriodDuration());
}

float erEcgTimer::getPeriodPosition(){
    return currentPeriodPosition.val();
}

float erEcgTimer::getPeriodDuration(){
    return currentPeriodDuration * 0.001;
}

float erEcgTimer::getCurrentBpm(){
    return currentBpm.val();
}

float erEcgTimer::getPercentThroughBpmArc(){
    return currentBpm.getPercentDone();
}

bool erEcgTimer::isWithinEcgBeepPeriod(){
    return isWithinEcgBeepPeriod(currentPeriodPosition.val());
}

bool erEcgTimer::isWithinEcgBeepPeriod(float position){
    return position > settings.ecgBeginBeepAt && position < settings.ecgEndBeepAt;
}

ofEvent<ofxAnimatable::AnimationEvent>& erEcgTimer::bpmLooped(){
    return currentBpm.animLooped;
}

void erEcgTimer::pauseBpmAnimation(){
    currentBpm.pause();
}

void erEcgTimer::resumeBpmAnimation(){
    currentBpm.resume();
}

void erEcgTimer::setDurationFromBpm(){
    currentPeriodDuration = 60000 / currentBpm.val();
}
