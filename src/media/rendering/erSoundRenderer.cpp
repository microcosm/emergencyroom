#include "erSoundRenderer.h"

void erSoundRenderer::setup(){
    isSetup = true;
    erGlitchRenderer::setup();
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

    for(int channel = 1; channel <= settings.numChannels; channel++){
        float volume = withinGlitchPeriod(currentTime, channel) ? settings.masterVolume : 0;
        staticSynths.at(channel - 1).set(Massive_master_volume, volume);
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

float erSoundRenderer::getCurrentEcgPosition(){
    timeSinceSync = currentTime - syncTime;
    currentEcgPosition = fmod(timeSinceSync, settings.ecgPeriod);
    return currentEcgPosition;
}