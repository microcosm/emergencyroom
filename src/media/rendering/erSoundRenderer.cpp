#include "erSoundRenderer.h"

void erSoundRenderer::setup(){
    isSetup = true;

    bpm = 5;
    period = 60000 / bpm;
    startOffset = period * 0.4;
    endOffset = period * 0.5;
    syncTime = 0;

    manager.setup();
    manager.toggleDebugUI();
    manager.bpm.setBpm(bpm);

    ecgSynth.setup("ECG", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&ecgChain).link(&ecgSynth).toMixer();

    staticSynth.setup("Static", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&staticChain).link(&staticSynth).toMixer();

    ofAddListener(ofEvents().update, this, &erSoundRenderer::update);
    ofAddListener(manager.bpm.beatEvent, this, &erSoundRenderer::play);
    manager.bpm.start();
    ecgChain.sendMidiOn(60);
}

void erSoundRenderer::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSoundRenderer::update(ofEventArgs& args){
    currentTime = ofGetElapsedTimeMillis();
    staticSynth.set(Massive_master_volume, withinGlitchPeriod() ? masterVolume : 0);
    ecgSynth.set(Massive_master_volume, withinEcgBeepPeriod() ? 1 : 0);
}

void erSoundRenderer::play(void){
    staticChain.sendMidiOn(60);
}

void erSoundRenderer::syncEcg(float delay){
    schedule(delay);
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

bool erSoundRenderer::withinEcgBeepPeriod(){
    if(syncTime > 0){
        timeSinceSync = currentTime - syncTime;
        float position = fmod(timeSinceSync, period);
        return position > startOffset && position < endOffset;
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