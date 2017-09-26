#pragma once
#include "ofMain.h"
#include "erTimedPlayer.h"
#include "erSoundRenderer.h"
#ifdef __linux__
#include "ofxOMXPlayer.h"
#endif

class erVideoPlayer : public erTimedPlayer{

public:
    void setup(string absolutePath, string relativePath, int volume, ofLoopType loopType){
        path = relativePath;
#ifdef __linux__
        ofxOMXPlayerSettings settings;
        settings.videoPath = absolutePath;
        settings.enableLooping = false;
        settings.enableAudio = false;
        settings.enableTexture = true; //really? .. default is true anyway
        omxPlayer.setup(settings);
        omxPlayer.setPaused(true);
#else
        videoPlayer.load(absolutePath);
        videoPlayer.setVolume(volume);
        videoPlayer.setLoopState(loopType);
#endif
    }

    void update(){
#ifdef __linux__
        //No need for OMX
#else
        videoPlayer.update();
#endif
    }

    void draw(float x, float y, float width, float height){
#ifdef __linux__
        if(!omxPlayer.isTextureEnabled()){
            return;
        }
        omxPlayer.draw(x, y, width, height);
#else
        videoPlayer.draw(x, y, width, height);
#endif
    }

    void stop(){
#ifdef __linux__
        omxPlayer.restartMovie();
        omxPlayer.setPaused(true);
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
        stop();
    }

    string getPath(){
        return path;
    }

    float getDuration() {
#ifdef __linux__
        return omxPlayer.getDurationInSeconds();
#else
        return videoPlayer.getDuration();
#endif
    }

    bool getIsMovieDone(){
#ifdef __linux__
        return omxPlayer.isPaused();
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
        return !omxPlayer.isPaused();
#else
        return videoPlayer.isPlaying();
#endif
    }

protected:
#ifdef __linux__
    ofxOMXPlayer omxPlayer;
#else
    ofVideoPlayer videoPlayer;
#endif
    erSoundRenderer* soundRenderer;
    bool useSoundRenderer = false;
    string path;

    void beginVideoPlayback(){
#ifdef __linux__
        omxPlayer.setPaused(false);
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
