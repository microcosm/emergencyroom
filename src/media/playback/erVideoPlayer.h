#pragma once
#include "ofMain.h"
#include "erTimedPlayer.h"
#include "erSoundRenderer.h"
#include "erOmxManager.h"

class erVideoPlayer : public erTimedPlayer{

public:
    void setup(string _absolutePath, string _relativePath, int volume, ofLoopType loopType, erOmxManager* _omxManager){
        relativePath = _relativePath;
        absolutePath = _absolutePath;
        omxManager = _omxManager;
#ifdef __linux__
        durationInSeconds = omxManager->getDuration(absolutePath);
#else
        videoPlayer.load(absolutePath);
        videoPlayer.setVolume(volume);
        videoPlayer.setLoopState(loopType);
#endif
    }

    void update(){
#ifdef __linux__
        
#else
        videoPlayer.update();
#endif
    }

    void draw(float x, float y, float width, float height){
#ifdef __linux__
        
#else
        videoPlayer.draw(x, y, width, height);
#endif
    }

    void stop(){
#ifdef __linux__
        //Nothing
#else
        if(isCurrentlyPlaying()){
            try{
                videoPlayer.stop();
            }catch(...){
                erLog("erVideoPlayer::stop()", "Caught videoPlayer.stop() exception");
            }
        }
        try{
            videoPlayer.setPosition(0);
        }catch(...){
            erLog("erVideoPlayer::stop()", "Caught videoPlayer.setPosition(0) exception");
        }
#endif
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
#ifdef __linux__
        omxManager->prepare(absolutePath);
#else
        stop();
#endif
    }

    string getPath(){
        return relativePath;
    }

    float getDuration() {
#ifdef __linux__
        return durationInSeconds;
#else
        return videoPlayer.getDuration();
#endif
    }

    bool getIsMovieDone(){
#ifdef __linux__
        throw invalid_argument("This path should never be reached.");
#else
        return videoPlayer.getIsMovieDone();
#endif
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
        throw invalid_argument("This path should never be reached.");
#else
        return videoPlayer.isPlaying();
#endif
    }

protected:
#ifdef __linux__
    float durationInSeconds;
#else
    ofVideoPlayer videoPlayer;
#endif
    erOmxManager* omxManager;
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string absolutePath;
    string relativePath;

    void beginVideoPlayback(){
#ifdef __linux__
        omxManager->begin();
#else
        videoPlayer.setSpeed(params.getSpeed());
        if(!isCurrentlyPlaying()) videoPlayer.play();
#endif
    }

    void beginSoundPlayback(){
        if(useSoundRenderer){
            soundRenderer->playVideoSound(params.getPath());
        }
    }
};
