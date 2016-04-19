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
    void newOpeningGlitchPeriod(unsigned long long from, float duration);
    void newClosingGlitchPeriod(unsigned long long from, float duration);
    bool withinGlitchPeriod();

protected:
    void initializeChannels();
    ofxAudioUnitManager manager;
    ofxAudioUnitChain chain;
    ofxManagedAudioUnit massive;

    map<int, int64_t> channelsToOpeningGlitchStarts;
    map<int, int64_t> channelsToOpeningGlitchEnds;
    map<int, int64_t> channelsToClosingGlitchStarts;
    map<int, int64_t> channelsToClosingGlitchEnds;
    int64_t currentTime;

    int numChannels, currentChannel;
    float masterVolume = 0;
    bool isSetup = false;
};