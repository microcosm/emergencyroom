#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erEcgTimer.h"
#include "erFileUtils.h"
#include "ofxAudioUnitManager.h"
#include "erSyncedMediaPlayer.h"
#include "erGlitchTimer.h"

#ifdef __APPLE__
    class erSoundRenderer : public erSyncedMediaPlayer, public erGlitchTimer{

    public:
        void setup();
        void setupEcg();
        void setupStatic();
        void setupBreathing();
        void setupVideo(vector<string>& audibleVideos);
        void ensureSetup();
        bool isSetup();

        void update();
        void draw();
        void syncEcg(float delay);
        bool isSyncing();
        bool hasSynced();
        void playVideoSound(string videoPath);
        void stopVideoSound(string videoPath);
        void stopVideoSound();
        erEcgTimer* getEcgTimer();

    protected:
        void beginPlayback(){
            lock();
            syncing = false;
            syncTime = ofGetElapsedTimeMillis();
            ecgTimer.start();
            unlock();
        }

        ofxAudioUnitManager manager;
        ofxManagedAudioUnit ecgSynth;
        ofxAudioUnitChain ecgChain;

        ofxManagedAudioUnit staticSynth;
        ofxAudioUnitChain staticChain;
        vector<ofxManagedAudioUnit> staticSynths;
        vector<ofxAudioUnitChain> staticChains;
        ofxManagedAudioUnitMixer staticMixer;

        ofxAudioUnit breathingEq;
        ofxAudioUnitFilePlayer breathingPlayer;

        ofxAudioUnitFilePlayer videoPlayer;
        ofxAudioUnitChain videoChain;
        map<string, ofxAudioUnitFilePlayer> videoPlayers;
        map<string, ofxAudioUnitChain> videoChains;
        ofxManagedAudioUnitMixer videoMixer;

        u_int64_t currentTime, syncTime, timeSinceSync;

        bool _isSetup = false;
        bool syncing;
        erEcgTimer ecgTimer;
    };
#endif