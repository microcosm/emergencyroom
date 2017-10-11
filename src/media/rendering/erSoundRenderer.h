#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erEcgTimer.h"
#include "erUtils.h"
#include "erThreadedPlayer.h"
#include "erGlitchTimer.h"

#ifdef __APPLE__
#include "ofxAudioUnitManager.h"
#endif

class erSoundRenderer : public erThreadedPlayer, public erGlitchTimer{

public:
    void setup(erEcgTimer* _ecgTimer);
    void setupEcg();
    void setupStatic();
    void setupBreathing();
    void setupVideo(vector<string>& audibleVideos);

    void update();
    void draw();
    void playVideoSound(string videoPath);
    void stopVideoSound(string videoPath);
    void stopVideoSound();

protected:
    
#ifdef __APPLE__
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
#endif

    u_int64_t currentTime;
    erEcgTimer* ecgTimer;
};
