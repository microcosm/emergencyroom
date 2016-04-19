#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"

class erSoundRenderer{

public:
    void setup();
    void ensureSetup();
    virtual void update(ofEventArgs& args);
    void play();
    void setMasterVolume(float _masterVolume);
    void setNumChannels(int _numChannels);
    void setCurrentChannel(int _currentChannel);
    void newOpeningGlitchPeriod(u_int64_t from, float duration);
    void newClosingGlitchPeriod(u_int64_t from, float duration);
    bool withinGlitchPeriod();

protected:
    void initializeChannels();
    ofxAudioUnitManager manager;
    ofxAudioUnitChain chain;
    ofxManagedAudioUnit massive;

    map<int, u_int64_t> channelsToOpeningGlitchStarts;
    map<int, u_int64_t> channelsToOpeningGlitchEnds;
    map<int, u_int64_t> channelsToClosingGlitchStarts;
    map<int, u_int64_t> channelsToClosingGlitchEnds;
    u_int64_t currentTime;

    int numChannels, currentChannel;
    float masterVolume = 0;
    bool isSetup = false;
};