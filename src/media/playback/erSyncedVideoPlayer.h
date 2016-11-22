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
        if(isCurrentlyPlaying()){
            try{
                stop();
            }catch(...){
                erLog("erSyncedVideoPlayer::before()", "Caught player stop() exception");
            }
        }
        try{
            setPosition(0);
        }catch(...){
            erLog("erSyncedVideoPlayer::before()", "Caught player setPosition(0) exception");
        }
    }

    string getPath(){
        return path;
    }

    bool isOrWillBePlaying(){
        return videoScheduled || isCurrentlyPlaying();
    }

    void renderSoundWith(erSoundRenderer* _soundRenderer){
        soundRenderer = _soundRenderer;
        useSoundRenderer = true;
    }

    bool isCurrentlyPlaying() {
#ifdef __linux__
        return !isPaused() && !getIsMovieDone();
#else
        return isPlaying();
#endif
    }

protected:
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string path;

    void beginVideoPlayback(){
        setSpeed(params.getSpeed());
        if(!isCurrentlyPlaying()) play();
    }

    void beginSoundPlayback(){
        if(useSoundRenderer){
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};
