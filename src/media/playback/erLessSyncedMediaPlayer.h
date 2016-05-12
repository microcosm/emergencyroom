#pragma once
#include "ofMain.h"
#include "erPlayParams.h"

class erLessSyncedMediaPlayer{

public:
    virtual void before(){};
    virtual void beginPlayback(){};
    virtual bool isOrWillBePlaying(){};

    void execute(erPlayParams _params){
        params = _params;
        schedule(params.getDelay());
    }

protected:
    u_int64_t playTime;
    erPlayParams params;
    bool scheduled = false;

    void schedule(u_int64_t delay){
        //cout << "now is: " << ofGetElapsedTimeMillis() << endl;
        scheduled = true;
        playTime = ofGetElapsedTimeMillis() + delay;
        //cout << "playTime is: " << playTime << endl;
        before();
    }
};