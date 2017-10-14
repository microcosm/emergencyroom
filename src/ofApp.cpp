#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    erSetLogLevels();
    ofSetFrameRate(60);
    ofSetWindowShape(1200, 800);
    settings.load();

    videoSoundAssigned = false;
    toggleFullscreen();
    ofHideCursor();

    mediaController.setVideoPaths(&mediaLoader.allVideos);
    mediaController.setVideoPlayers(&mediaLoader.videoPlayers);

    mediaLoader.setup(&network);
    mediaController.setup(&network, &ecgTimer);
    network.setup();
    mediaSequencer.setup(&network, &mediaLoader, &mediaController, &ecgTimer);
    statusRenderer.setup(&network, &mediaSequencer, &mediaController);

    if(settings.isServer){
        clientController.setup(&network);
        ecgTimer.setup();
    }
}

void ofApp::update(){
    if(settings.isServer){
        clientController.openClientApps();
    }

    if(mediaLoader.isLoaded() && !videoSoundAssigned){
        mediaController.useSoundRendererFor(mediaLoader.audibleVideos);
        videoSoundAssigned = true;
    }

    if(settings.logToFileEnabled){
        erEnableFileLogging(settings.isServer);
    }else{
        erDisableFileLogging();
    }

    if(settings.isServer){
        ecgTimer.update();
    }

    mediaLoader.update();
    mediaController.update();
    network.update();
    mediaSequencer.update();
}

void ofApp::draw(){
    mediaController.draw();
    statusRenderer.draw();

    if(mediaLoader.hasErrors()){
        mediaLoader.drawErrors();
    }
}

void ofApp::keyReleased(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'n'){
        settings.incrementServerViewMode();
    }
    if(key == 'N'){
        settings.incrementClientViewMode();
        settings.renderStatus() ? network.clientStatusOn() : network.clientStatusOff();
    }
    if(key == ' '){
        mediaSequencer.stopAll();
    }
}

void ofApp::toggleFullscreen(){
    if(settings.isServer){
        if(settings.fullscreenServer){
            ofToggleFullscreen();
        }
    }else if(settings.fullscreenClient){
        ofToggleFullscreen();
    }
}

void ofApp::exit(){
    clientController.closeClientApps();
}
