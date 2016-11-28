#pragma once
#include "ofMain.h"
#include "erPlayParams.h"

class erTimedPlayer{

public:
    virtual void before(){};
    virtual void beginVideoPlayback(){};
    virtual void beginSoundPlayback(){};
    virtual bool isOrWillBePlaying(){};

    void execute(erPlayParams _params){
        params = _params;
        schedule(params.getDelay());
    }

protected:
    u_int64_t videoPlayTime, soundPlayTime;
    erPlayParams params;
    bool videoScheduled = false;
    bool soundScheduled = false;
    int soundDelay = 200;

    void schedule(u_int64_t delay){
        videoScheduled = true;
        soundScheduled = true;
        videoPlayTime = ofGetElapsedTimeMillis() + delay;
        soundPlayTime = videoPlayTime + soundDelay;
        before();
    }
};
