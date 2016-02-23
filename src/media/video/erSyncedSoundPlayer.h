#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"

class erSyncedSoundPlayer : public ofSoundPlayer, public erSyncedMediaPlayer{
    void beginPlayback(){
        setSpeed(params.getSpeed());
        play();
    }
};