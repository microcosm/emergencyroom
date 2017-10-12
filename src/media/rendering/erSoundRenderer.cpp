#include "erSoundRenderer.h"

void erSoundRenderer::setup(erEcgTimer* _ecgTimer){
#ifdef __APPLE__
    ecgTimer = _ecgTimer;
    erGlitchTimer::setup();

    manager.setup();
    setupEcg();
    setupStatic();
    setupBreathing();
#endif
}

void erSoundRenderer::setupEcg(){
#ifdef __APPLE__
    ecgSynth.setup("ECG", AUDIOUNIT_MASSIVE);
    manager.createChain(&ecgChain, "ecg").link(&ecgSynth).toMixer();
    ecgChain.sendMidiOn(60);
#endif
}

void erSoundRenderer::setupStatic(){
#ifdef __APPLE__
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
#endif
}

void erSoundRenderer::setupBreathing(){
#ifdef __APPLE__
    breathingPlayer.setFile(ofToDataPath(settings.breathingSoundPath));
    breathingPlayer.loop();
    breathingEq.setup('aufx', 'nbeq', 'appl');
    breathingEq.setParameter(0, kAudioUnitScope_Global, ofMap(settings.breathingVolume, 0, 1, -96, 0));
    breathingPlayer.connectTo(breathingEq);
    manager.addUnmanagedUnit(&breathingEq);
#endif
}

void erSoundRenderer::setupVideo(vector<string>& audibleVideos){
#ifdef __APPLE__
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
#endif
}

void erSoundRenderer::update(){
#ifdef __APPLE__
    currentTime = ofGetElapsedTimeMillis();
    ecgSynth.set(Massive_master_volume, ecgTimer->isWithinEcgBeepPeriod() ? settings.ecgVolume : 0);

    for(int channel = 1; channel <= settings.numChannels; channel++){
        float volume = withinGlitchPeriod(channel, currentTime) ? settings.staticVolume : 0;
        staticSynths.at(channel - 1).set(Massive_master_volume, volume);
    }
#endif
}

void erSoundRenderer::draw(){
#ifdef __APPLE__
    manager.enableDebugUI(settings.renderAudioUI());

    if(settings.renderAudioUI()){
        float progressThroughPeriod;
        int progressOnScreen = ofMap(ecgTimer->getPeriodPosition(), 0, 1, 0, ofGetWidth());
        for(int x = 0; x < progressOnScreen; x++){
            progressThroughPeriod = ofMap(x, 0, ofGetWidth(), 0, 1);
            ofSetColor(ecgTimer->isWithinEcgBeepPeriod(progressThroughPeriod) ? ofColor::green : ofColor::white);
            ofDrawRectangle(x, ofGetHeight(), 1, -2);
        }
    }
#endif
}

void erSoundRenderer::playVideoSound(string videoPath){
#ifdef __APPLE__
    videoPlayers[videoPath].play();
#endif
}

void erSoundRenderer::stopVideoSound(string videoPath){
#ifdef __APPLE__
    videoPlayers[videoPath].stop();
#endif
}

void erSoundRenderer::stopVideoSound(){
#ifdef __APPLE__
    for(auto const& player : videoPlayers){
        stopVideoSound(player.first);
    }
#endif
}
