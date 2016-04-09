#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"

class erSyncedVideoPlayer : public ofVideoPlayer, public erSyncedMediaPlayer{

public:
    bool isOrWillBePlaying(){
        return scheduled || isPlaying();
    }

protected:
    void beforeSleep(){
        stop();
    }

    void beginPlayback(){
        setSpeed(params.getSpeed());
        play();
    }
};