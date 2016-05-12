#pragma once
#include "ofMain.h"
#include "erLessSyncedMediaPlayer.h"
#include "erSoundRenderer.h"

class erSyncedVideoPlayer : public ofVideoPlayer, public erLessSyncedMediaPlayer{

public:
    void setPath(string _path){
        path = _path;
    }

    void checkSchedule(){
        if(scheduled && ofGetElapsedTimeMillis() > playTime){
            beginPlayback();
            scheduled = false;
        }
    }

    void before(){
        if(isPlaying()){
            stop();
        }
        setPosition(0);
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
    int soundDelay = 103;
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string path;

    void beginPlayback(){
        setSpeed(params.getSpeed());
        if(!isPlaying()) play();

        if(useSoundRenderer){
            //ofSleepMillis(soundDelay);
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};