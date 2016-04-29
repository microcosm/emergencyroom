#include "erSoundRenderer.h"

void erSoundRenderer::setup(){
    isSetup = true;
    syncing = syncedBefore = false;

    startOffset = settings.ecgPeriod * settings.ecgBeginBeepAt;
    endOffset = settings.ecgPeriod * settings.ecgEndBeepAt;
    syncTime = 0;

    manager.setup();
    manager.bpm.setBpm(settings.ecgBpm);

    ecgSynth.setup("ECG", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&ecgChain).link(&ecgSynth).toMixer();

    staticSynth.setup("Static", 'aumu', 'NiMa', '-NI-');
    manager.createChain(&staticChain).link(&staticSynth).toMixer();

    ofAddListener(ofEvents().update, this, &erSoundRenderer::update);
    ofAddListener(ofEvents().draw, this, &erSoundRenderer::draw);
    manager.bpm.start();
    ecgChain.sendMidiOn(60);
    staticChain.sendMidiOn(60);

    initializeChannels();
}

void erSoundRenderer::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSoundRenderer::update(ofEventArgs& args){
    currentTime = ofGetElapsedTimeMillis();
    currentEcgPosition = getCurrentEcgPosition();
    staticSynth.set(Massive_master_volume, withinGlitchPeriod(currentTime) ? settings.masterVolume : 0);
    ecgSynth.set(Massive_master_volume, withinEcgBeepPeriod(currentEcgPosition) ? settings.masterVolume : 0);
}

void erSoundRenderer::draw(ofEventArgs& args){
    if(settings.drawingEnabled && hasSyncedBefore()){
        float progressThroughPeriod;
        int progressOnScreen = ofMap(currentEcgPosition, 0, settings.ecgPeriod, 0, ofGetWidth());
        for(int x = 0; x < progressOnScreen; x++){
            progressThroughPeriod = ofMap(x, 0, ofGetWidth(), 0, settings.ecgPeriod);
            ofSetColor(withinEcgBeepPeriod(progressThroughPeriod) ? ofColor::green : ofColor::white);
            ofDrawRectangle(x, ofGetHeight(), 1, -2);
        }
    }
}

void erSoundRenderer::syncEcg(float delay){
    schedule(delay);
    syncing = true;
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

bool erSoundRenderer::withinGlitchPeriod(u_int64_t time){
    for(int i = 1; i <= settings.numChannels; i++){
        if(time > channelsToOpeningGlitchStarts[i] && time < channelsToOpeningGlitchEnds[i]){
            return true;
        }

        if(time > channelsToClosingGlitchStarts[i] && time < channelsToClosingGlitchEnds[i]){
            return true;
        }
    }
    return false;
}

bool erSoundRenderer::withinEcgBeepPeriod(float position){
    if(hasSyncedBefore()){
        return position > startOffset && position < endOffset;
    }
    return false;
}

bool erSoundRenderer::isSyncing(){
    return syncing;
}

bool erSoundRenderer::hasSyncedBefore(){
    return syncedBefore;
}

void erSoundRenderer::initializeChannels(){
    for(int i = 1; i <= settings.numChannels; i++){
        channelsToOpeningGlitchStarts[i] = 0;
        channelsToOpeningGlitchEnds[i] = 0;
        channelsToClosingGlitchStarts[i] = 0;
        channelsToClosingGlitchEnds[i] = 0;
    }
}

float erSoundRenderer::getCurrentEcgPosition(){
    timeSinceSync = currentTime - syncTime;
    currentEcgPosition = fmod(timeSinceSync, settings.ecgPeriod);
    return currentEcgPosition;
}