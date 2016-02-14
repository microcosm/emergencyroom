#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"
#include "ofxHapPlayer.h"

class erSyncedHapPlayer : public ofxHapPlayer, public erSyncedMediaPlayer{
    void beforeSleep(){
        stop();
    }

    void beginPlayback(){
        setPosition(0);
        play();
    }
};