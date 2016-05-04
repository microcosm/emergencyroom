#include "erSoundRenderer.h"

void erSoundRenderer::setup(){
    isSetup = true;
    erGlitchRenderer::setup();
    syncing = syncedBefore = false;

    manager.setup();
    setupEcg();
    setupStatic();
    setupBreathing();

    ofAddListener(ofEvents().update, this, &erSoundRenderer::update);
    ofAddListener(ofEvents().draw, this, &erSoundRenderer::draw);
}

void erSoundRenderer::setupEcg(){
    startOffset = settings.ecgPeriod * settings.ecgBeginBeepAt;
    endOffset = settings.ecgPeriod * settings.ecgEndBeepAt;
    syncTime = 0;

    ecgSynth.setup("ECG", AUDIOUNIT_MASSIVE);
    manager.createChain(&ecgChain, "ecg").link(&ecgSynth).toMixer();
    ecgChain.sendMidiOn(60);
}

void erSoundRenderer::setupStatic(){
    for(int i = 0; i < settings.numChannels; i++){
        staticSynths.push_back(staticSynth);
        staticChains.push_back(staticChain);
    }

    for(int i = 0; i < settings.numChannels; i++){
        string name = "static" + ofToString(i + 1);
        staticSynths.at(i).setup(name, AUDIOUNIT_MASSIVE);
        manager.createChain(&staticChains.at(i), name, &staticMixer).link(&staticSynths.at(i)).toMixer();
        staticChains.at(i).sendMidiOn(60);
    }

    manager.addUnmanagedUnit(staticMixer.getUnit());
}

void erSoundRenderer::setupBreathing(){
    breathingPlayer.setFile(ofToDataPath(settings.breathingSoundPath));
    breathingPlayer.loop();
    breathingEq.setup('aufx', 'nbeq', 'appl');
    breathingEq.setParameter(0, kAudioUnitScope_Global, ofMap(settings.breathingVolume, 0, 1, -96, 0));
    breathingPlayer.connectTo(breathingEq);
    manager.addUnmanagedUnit(&breathingEq);
}

void erSoundRenderer::setupVideo(vector<string>& audibleVideos){
    if(videoPlayers.size() == 0){
        videoMixer.getUnit()->setInputBusCount(audibleVideos.size());
        int i = 0;
        for(auto const& video : audibleVideos){
            string name = "video" + ofToString(i + 1);
            videoPlayers[video] = videoPlayer;
            videoPlayers[video].setFile(ofToDataPath(settings.previewMediaPath + video));
            videoPlayers[video].connectTo(*videoMixer.getUnit(), i);
            videoMixer.getUnit()->setInputVolume(erGetVolume(video) * settings.videoVolume, i);
            i++;
        }

        manager.addUnmanagedUnit(videoMixer.getUnit());
    }
}

void erSoundRenderer::ensureSetup(){
    if(!isSetup){
        setup();
    }
}

void erSoundRenderer::update(ofEventArgs& args){
    currentTime = ofGetElapsedTimeMillis();
    currentEcgPosition = getCurrentEcgPosition();
    ecgSynth.set(Massive_master_volume, withinEcgBeepPeriod(currentEcgPosition) ? settings.ecgVolume : 0);

    for(int channel = 1; channel <= settings.numChannels; channel++){
        float volume = withinGlitchPeriod(channel, currentTime) ? settings.staticVolume : 0;
        staticSynths.at(channel - 1).set(Massive_master_volume, volume);
    }
}

void erSoundRenderer::draw(ofEventArgs& args){
    if(settings.serverDrawingEnabled && hasSyncedBefore()){
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

void erSoundRenderer::playSound(string videoPath){
    videoPlayers[videoPath].play();
}

float erSoundRenderer::getCurrentEcgPosition(){
    timeSinceSync = currentTime - syncTime;
    currentEcgPosition = fmod(timeSinceSync, settings.ecgPeriod);
    return currentEcgPosition;
}