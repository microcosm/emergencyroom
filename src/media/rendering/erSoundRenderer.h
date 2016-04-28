#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erSyncedMediaPlayer.h"

class erSoundRenderer : erSyncedMediaPlayer{

public:
    void setup();
    void ensureSetup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void play();
    void syncEcg(float delay);
    void setMasterVolume(float _masterVolume);
    void setNumChannels(int _numChannels);
    void setCurrentChannel(int _currentChannel);
    void newOpeningGlitchPeriod(u_int64_t from, float duration);
    void newClosingGlitchPeriod(u_int64_t from, float duration);
    bool withinGlitchPeriod(u_int64_t time);
    bool withinEcgBeepPeriod(float position);
    bool isSyncing();
    bool hasSyncedBefore();

protected:
    void beginPlayback(){
        manager.bpm.reset();
        syncTime = ofGetElapsedTimeMillis();
        syncing = false;
        syncedBefore = true;
    }

    void initializeChannels();
    float getCurrentEcgPosition();
    ofxAudioUnitManager manager;
    ofxAudioUnitChain staticChain, ecgChain;
    ofxManagedAudioUnit staticSynth, ecgSynth;

    map<int, u_int64_t> channelsToOpeningGlitchStarts;
    map<int, u_int64_t> channelsToOpeningGlitchEnds;
    map<int, u_int64_t> channelsToClosingGlitchStarts;
    map<int, u_int64_t> channelsToClosingGlitchEnds;
    u_int64_t currentTime, syncTime, timeSinceSync;

    float startOffset, endOffset, currentEcgPosition;

    int numChannels, currentChannel;
    float masterVolume = 0;
    bool isSetup = false;
    bool syncing, syncedBefore;
};