#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erEcgTimer.h"
#include "erFileUtils.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erSyncedMediaPlayer.h"
#include "erGlitchTimer.h"

class erSoundRenderer : erSyncedMediaPlayer, public erGlitchTimer{

public:
    void setup();
    void setupEcg();
    void setupStatic();
    void setupBreathing();
    void setupVideo(vector<string>& audibleVideos);
    void ensureSetup();

    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void syncEcg(float delay);
    bool isSyncing();
    bool hasSynced();
    void playSound(string videoPath);
    erEcgTimer* getEcgTimer();

protected:
    void beginPlayback(){
        syncing = false;
        syncTime = ofGetElapsedTimeMillis();
        ecgTimer.start();
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

    bool isSetup = false;
    bool syncing;
    erEcgTimer ecgTimer;
};