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
    manager.createChain(&ecgChain, "ecg").link(&ecgSynth).toMixer();
    ecgChain.sendMidiOn(60);

    for(int i = 0; i < settings.numChannels; i++){
        staticSynths.push_back(staticSynth);
        staticChains.push_back(staticChain);
    }

    for(int i = 0; i < settings.numChannels; i++){
        string name = "static" + ofToString(i + 1);
        staticSynths.at(i).setup(name, 'aumu', 'NiMa', '-NI-');
        manager.createChain(&staticChains.at(i), name).link(&staticSynths.at(i)).toMixer();
        staticChains.at(i).sendMidiOn(60);
    }

    initializeChannels();

    ofAddListener(ofEvents().update, this, &erSoundRenderer::update);
    ofAddListener(ofEvents().draw, this, &erSoundRenderer::draw);
}

void erSoundRenderer::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSoundRenderer::update(ofEventArgs& args){
    currentTime = ofGetElapsedTimeMillis();
    currentEcgPosition = getCurrentEcgPosition();
    ecgSynth.set(Massive_master_volume, withinEcgBeepPeriod(currentEcgPosition) ? settings.masterVolume : 0);

    for(int i = 1; i <= settings.numChannels; i++){
        staticSynths.at(i - 1).set(Massive_master_volume, withinGlitchPeriod(i, currentTime) ? settings.masterVolume : 0);
    }
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

void erSoundRenderer::newOpeningGlitchPeriod(int channel, u_int64_t from, float duration){
    channelsToOpeningGlitchStarts[channel] = from;
    channelsToOpeningGlitchEnds[channel] = from + duration;
}

void erSoundRenderer::newClosingGlitchPeriod(int channel, u_int64_t from, float duration){
    channelsToClosingGlitchStarts[channel] = from;
    channelsToClosingGlitchEnds[channel] = from + duration;
}

bool erSoundRenderer::withinGlitchPeriod(int channel, u_int64_t time){
    if(time > channelsToOpeningGlitchStarts[channel] && time < channelsToOpeningGlitchEnds[channel]){
        return true;
    }

    if(time > channelsToClosingGlitchStarts[channel] && time < channelsToClosingGlitchEnds[channel]){
        return true;
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