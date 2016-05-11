#pragma once
#include "ofMain.h"
#include "erSyncedMediaPlayer.h"
#include "erSoundRenderer.h"

//functions / vars in ofVideoPlayer are shared!

class erSyncedVideoPlayer : public ofVideoPlayer, public erSyncedMediaPlayer{

public:
    void setPath(string _path){
        path = _path;
    }

    string getPath(){
        return path;
    }

    bool isOrWillBePlaying(){
        lock();
        bool isOrWillBe = scheduled || isPlaying();
        unlock();
        return isOrWillBe;
    }

    void renderSoundWith(erSoundRenderer* _soundRenderer){
        soundRenderer = _soundRenderer;
        useSoundRenderer = true;
    }

protected:
    int soundDelay = 103; //shared, but value only assigned once
    erSoundRenderer* soundRenderer; //shared, but pointer only assigned once
    bool useSoundRenderer = false; //shared, but bool and only assigned once
    string path; //not shared

    //runs in thread
    void beforeSleep(){
        lock();
        stop();
        unlock();
    }

    //runs in thread
    void beginPlayback(){
        lock();
        setSpeed(params.getSpeed());
        if(!isPlaying()) play();
        unlock();

        if(useSoundRenderer){
            ofSleepMillis(soundDelay);
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};