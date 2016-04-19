#include "erSoundRenderer.h"

void erSoundRenderer::setup(){
    isSetup = true;
    manager.setup();
    manager.toggleDebugUI();
    massive.setup("Massive", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&chain).link(&massive).toMixer();

    ofAddListener(ofEvents().update, this, &erSoundRenderer::update);
    ofAddListener(manager.bpm.beatEvent, this, &erSoundRenderer::play);
    manager.bpm.start();
}

void erSoundRenderer::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSoundRenderer::update(ofEventArgs& args){
    massive.set(Massive_master_volume, withinGlitchPeriod() ? masterVolume : 0);
}

void erSoundRenderer::play(void){
    chain.sendMidiOn(60);
}

void erSoundRenderer::setMasterVolume(float _masterVolume){
    masterVolume = _masterVolume;
}

void erSoundRenderer::setNumChannels(int _numChannels){
    numChannels = _numChannels;
    initializeChannels();
}

void erSoundRenderer::setCurrentChannel(int _currentChannel){
    currentChannel = _currentChannel;
}

void erSoundRenderer::newOpeningGlitchPeriod(u_int64_t from, float duration){
    channelsToOpeningGlitchStarts[currentChannel] = from;
    channelsToOpeningGlitchEnds[currentChannel] = from + duration;
}

void erSoundRenderer::newClosingGlitchPeriod(u_int64_t from, float duration){
    channelsToClosingGlitchStarts[currentChannel] = from;
    channelsToClosingGlitchEnds[currentChannel] = from + duration;
}

bool erSoundRenderer::withinGlitchPeriod(){
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

void erSoundRenderer::initializeChannels(){
    for(int i = 1; i <= numChannels; i++){
        channelsToOpeningGlitchStarts[i] = 0;
        channelsToOpeningGlitchEnds[i] = 0;
        channelsToClosingGlitchStarts[i] = 0;
        channelsToClosingGlitchEnds[i] = 0;
    }
}