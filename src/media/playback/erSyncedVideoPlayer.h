#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"
#include "erSoundRenderer.h"

class erSyncedVideoPlayer : public ofVideoPlayer, public erSyncedMediaPlayer{

public:
    void setPath(string _path){
        path = _path;
    }

    string getPath(){
        return path;
    }

    bool isOrWillBePlaying(){
        return scheduled || isPlaying();
    }

    void renderSoundWith(erSoundRenderer* _soundRenderer){
        soundRenderer = _soundRenderer;
        useSoundRenderer = true;
    }

protected:
    bool useSoundRenderer = false;
    erSoundRenderer* soundRenderer;
    int soundDelay = 103;
    string path;

    void beforeSleep(){
        stop();
    }

    void beginPlayback(){
        setSpeed(params.getSpeed());
        if(!isPlaying()) play();

        if(useSoundRenderer){
            ofSleepMillis(soundDelay);
            lock();
            soundRenderer->playSound(params.getPath());
            unlock();
        }
    }
};