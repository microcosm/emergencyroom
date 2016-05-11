#pragma once
#include "ofMain.h"
#include "erPlayParams.h"

class erSyncedMediaPlayer : public ofThread{

public:
    virtual void beforeSleep(){};
    virtual void beginPlayback(){};
    virtual bool isOrWillBePlaying(){};

    void schedule(u_int64_t delay){
        scheduled = true;
        lock();
        playTime = ofGetElapsedTimeMillis() + delay;
        unlock();
        startThread(true);
    }

    void execute(erPlayParams _params){
        lock();
        params = _params;
        schedule(params.getDelay());
        unlock();
    }

protected:
    u_int64_t playTime; //shared!
    erPlayParams params; //shared!
    bool scheduled = false; //shared, but bool and only assigned once

    //runs in thread
    void threadedFunction(){
        lock();
        u_int64_t delay = playTime - ofGetElapsedTimeMillis();
        unlock();
        beforeSleep();
        ofSleepMillis(delay);
        scheduled = false;
        beginPlayback();
    }
};