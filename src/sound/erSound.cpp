#include "erSound.h"

void erSound::setup(){
    isSetup = true;
    manager.setup();
    manager.toggleDebugUI();
    massive.setup("Massive", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&chain).link(&massive).toMixer();

    ofAddListener(ofEvents().update, this, &erSound::update);
    ofAddListener(manager.bpm.beatEvent, this, &erSound::play);
    manager.bpm.start();
}

void erSound::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSound::update(ofEventArgs& args){
    massive.set(Massive_master_volume, withinGlitchPeriod() ? 1 : 0);
}

void erSound::play(void){
    chain.sendMidiOn(60);
}

void erSound::setNumChannels(int _numChannels){
    numChannels = _numChannels;
    initializeChannels();
}

void erSound::setCurrentChannel(int _currentChannel){
    currentChannel = _currentChannel;
}

void erSound::newOpeningGlitchPeriod(unsigned long long from, float duration){
    channelsToOpeningGlitchStarts[currentChannel] = from;
    channelsToOpeningGlitchEnds[currentChannel] = from + duration;
}

void erSound::newClosingGlitchPeriod(unsigned long long from, float duration){
    channelsToClosingGlitchStarts[currentChannel] = from;
    channelsToClosingGlitchEnds[currentChannel] = from + duration;
}

bool erSound::withinGlitchPeriod(){
    currentTime = ofGetElapsedTimeMillis();
    for(int i = 1; i <= numChannels; i++){
        if(currentTime > channelsToOpeningGlitchStarts[i] && currentTime < channelsToOpeningGlitchEnds[i]){
            return true;
        }

        if(currentTime > channelsToClosingGlitchStarts[i] && currentTime < channelsToClosingGlitchEnds[i]){
            return true;
        }
    }
    return false;
}

void erSound::initializeChannels(){
    for(int i = 1; i <= numChannels; i++){
        channelsToOpeningGlitchStarts[i] = 0;
        channelsToOpeningGlitchEnds[i] = 0;
        channelsToClosingGlitchStarts[i] = 0;
        channelsToClosingGlitchEnds[i] = 0;
    }
}