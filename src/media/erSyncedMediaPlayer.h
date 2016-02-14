#pragma once
#include "ofMain.h"

class erSyncedMediaPlayer : public ofThread{

    int playTime;

    void threadedFunction(){
        int delay = playTime - ofGetElapsedTimeMillis();
        beforeSleep();
        ofSleepMillis(delay);
        beginPlayback();
    }

public:
    virtual void beforeSleep(){};
    virtual void beginPlayback(){};

    void schedule(int delay){
        playTime = ofGetElapsedTimeMillis() + delay;
        startThread(true);
    }
};