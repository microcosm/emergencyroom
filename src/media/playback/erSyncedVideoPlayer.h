#pragma once
#include "ofMain.h"
#include "erLessSyncedMediaPlayer.h"
#include "erSoundRenderer.h"

class erSyncedVideoPlayer : public erLessSyncedMediaPlayer{

public:
    void setup(string absolutePath, string relativePath, int volume, ofLoopType loopType){
        videoPlayer.load(absolutePath);
        path = relativePath;
        videoPlayer.setVolume(volume);
        videoPlayer.setLoopState(loopType);
    }

    void update(){
        videoPlayer.update();
    }

    void draw(float x, float y, float width, float height){
        videoPlayer.draw(x, y, width, height);
    }

    void stop(){
        if(isCurrentlyPlaying()){
            try{
                videoPlayer.stop();
            }catch(...){
                erLog("erSyncedVideoPlayer::stop()", "Caught videoPlayer.stop() exception");
            }
        }
        try{
            videoPlayer.setPosition(0);
        }catch(...){
            erLog("erSyncedVideoPlayer::stop()", "Caught videoPlayer.setPosition(0) exception");
        }
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
        stop();
    }

    string getPath(){
        return path;
    }

    float getDuration() {
        return videoPlayer.getDuration();
    }

    bool getIsMovieDone(){
        return videoPlayer.getIsMovieDone();
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
        return !videoPlayer.isPaused() && !videoPlayer.getIsMovieDone();
#else
        return videoPlayer.isPlaying();
#endif
    }

protected:
    ofVideoPlayer videoPlayer;
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string path;

    void beginVideoPlayback(){
        videoPlayer.setSpeed(params.getSpeed());
        if(!isCurrentlyPlaying()) videoPlayer.play();
    }

    void beginSoundPlayback(){
        if(useSoundRenderer){
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};
