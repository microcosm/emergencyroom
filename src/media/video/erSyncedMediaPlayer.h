#pragma once
#include "ofMain.h"
#include "erPlayParams.h"

class erSyncedMediaPlayer : public ofThread{

public:
    virtual void beforeSleep(){};
    virtual void beginPlayback(){};
    virtual bool isOrWillBePlaying(){};

    void schedule(int delay){
        scheduled = true;
        playTime = ofGetElapsedTimeMillis() + delay;
        startThread(true);
    }

    void execute(erPlayParams _params){
        params = _params;
        schedule(params.getDelay());
    }

protected:
    int playTime;
    erPlayParams params;
    bool scheduled = false;

    void threadedFunction(){
        int delay = playTime - ofGetElapsedTimeMillis();
        beforeSleep();
        ofSleepMillis(delay);
        scheduled = false;
        beginPlayback();
    }
};