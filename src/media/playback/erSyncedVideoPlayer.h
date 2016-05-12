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
        if(videoScheduled && ofGetElapsedTimeMillis() > videoPlayTime){
            beginVideoPlayback();
            videoScheduled = false;
        }
        if(soundScheduled && ofGetElapsedTimeMillis() > soundPlayTime){
            beginSoundPlayback();
            soundScheduled = false;
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
        return videoScheduled || isPlaying();
    }

    void renderSoundWith(erSoundRenderer* _soundRenderer){
        soundRenderer = _soundRenderer;
        useSoundRenderer = true;
    }

protected:
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string path;

    void beginVideoPlayback(){
        setSpeed(params.getSpeed());
        if(!isPlaying()) play();
    }

    void beginSoundPlayback(){
        if(useSoundRenderer){
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};