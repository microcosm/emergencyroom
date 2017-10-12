#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    erSetLogLevels();
    ofSetFrameRate(60);
    ofSetWindowShape(1200, 800);
    settings.load();

    bigFont.load(settings.fontPath, 75);
    smallFont.load(settings.fontPath, 50);
    videoSoundAssigned = false;

    toggleFullscreen();

    mediaController.setVideoPaths(&mediaLoader.allVideos);
    mediaController.setVideoPlayers(&mediaLoader.videoPlayers);

    mediaLoader.setup(&network);
    mediaController.setup(&network, &ecgTimer);
    network.setup();
    mediaSequencer.setup(&network, &mediaLoader, &mediaController, &ecgTimer);

    if(settings.isServer){
        clientController.setup(&network);
        ecgTimer.setup();
    }
}

void ofApp::update(){
    width = ofGetWidth();
    height = ofGetHeight();

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
    ofSetColor(ofColor::white);

    mediaController.draw();
    if(settings.isClient){
        ofSetColor(ofColor::black, 150);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    network.draw();
    mediaSequencer.draw();

    if(mediaLoader.hasErrors()){
        mediaLoader.drawErrors();
    }

    ofSetColor(ofColor::white);
    if(settings.isServer && settings.serverDrawingEnabled){
        ofDrawBitmapString("v            toggle audio unit manager\n\nd            toggle server display\n\nD            toggle client display\n\nup/down      select ecg client\n\n-            sync to ecg client", 130, height - 168);
        drawFps(490, height - 60);
        smallFont.drawString(mediaSequencer.getCurrentCollection(), 490, height - 220);
    }

    if(settings.isClient && settings.clientDrawingEnabled){
        ofDrawBitmapString("Computer name: '" + settings.computerName + "'\nDimensions:    " + ofToString(width) + " x " + ofToString(height), width - 350, 40);
        drawFps(320, height - 46);
    }

    if(settings.isClient && settings.clientDrawingEnabled){
        int clientId = network.getClientId();
        if(clientId != -1){
            ofDrawBitmapString("Client ID:", 50, height - 148);
            smallFont.drawString("#" + ofToString(network.getClientId()), 50, height - 90);
            ofDrawBitmapString("Playback state:", 50, height - 236);
            ofDrawBitmapString(mediaController.getClientVideoState(), 50, height - 220);
        }
    }
}

void ofApp::drawFps(int x, int y){
    bigFont.drawString("fps: " + ofToString(int(ofGetFrameRate())), x, y);
}

void ofApp::keyReleased(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'd'){
        settings.toggleServerDrawing();
    }
    if(key == 'D'){
        settings.toggleClientDrawing();
        settings.clientDrawingEnabled ? network.clientDisplaysOn() : network.clientDisplaysOff();
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
