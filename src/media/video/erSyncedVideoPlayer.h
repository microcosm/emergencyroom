#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"

class erSyncedVideoPlayer : public ofVideoPlayer, public erSyncedMediaPlayer{
    void beforeSleep(){
        stop();
    }

    void beginPlayback(){
        setSpeed(params.getSpeed());
        play();
    }
};