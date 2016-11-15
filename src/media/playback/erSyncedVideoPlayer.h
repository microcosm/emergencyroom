#pragma once
#include "ofMain.h"
#include "erLessSyncedMediaPlayer.h"
#ifdef __APPLE__
    #include "erSoundRenderer.h"
#endif

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
        return videoScheduled || isPlaying();
    }


    #ifdef __APPLE__
        void renderSoundWith(erSoundRenderer* _soundRenderer){
            soundRenderer = _soundRenderer;
            useSoundRenderer = true;
        }
    #endif

protected:
    #ifdef __APPLE__
        erSoundRenderer* soundRenderer;
    #endif
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