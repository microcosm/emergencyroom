#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erSyncedMediaPlayer.h"
#include "erGlitchRenderer.h"

class erSoundRenderer : erSyncedMediaPlayer, public erGlitchRenderer{

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
    bool withinEcgBeepPeriod(float position);
    bool isSyncing();
    bool hasSyncedBefore();
    void playSound(string videoPath);

protected:
    void beginPlayback(){
        manager.bpm.reset();
        syncTime = ofGetElapsedTimeMillis();
        syncing = false;
        syncedBefore = true;
    }

    float getCurrentEcgPosition();

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
    float startOffset, endOffset, currentEcgPosition;

    bool isSetup = false;
    bool syncing, syncedBefore;
};