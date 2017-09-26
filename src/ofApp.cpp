#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(1200, 800);
    bigFont.load(ER_FONT_PATH, 75);
    smallFont.load(ER_FONT_PATH, 50);
    videoSoundAssigned = false;
    settings.load();
    if(settings.fullscreenByDefault){
        ofToggleFullscreen();
    }

    mediaController.setVideoPaths(&mediaLoader.allVideos);
    mediaController.setVideoPlayers(&mediaLoader.videoPlayers);

    if(settings.isEcg){
        mediaController.setupEcgMode(&network);
        network.setup();
        ecg.setup(&network);
    }else{
        mediaLoader.setup(&network);
        mediaController.setup(&network);
        network.setup();
        mediaSequencer.setup(&network, &mediaLoader, &mediaController);
    }
}

void ofApp::update(){
    width = ofGetWidth();
    height = ofGetHeight();

    if(mediaLoader.isLoaded() && !videoSoundAssigned){
        mediaController.useSoundRendererFor(mediaLoader.audibleVideos);
        videoSoundAssigned = true;
    }

    if(settings.logToFileEnabled){
        erEnableFileLogging(settings.isServer);
    }else{
        erDisableFileLogging();
    }

    if(settings.isEcg){
        ecg.update();
        network.update();
    }else{
        mediaLoader.update();
        mediaController.update();
        network.update();
        mediaSequencer.update();
    }
}

void ofApp::draw(){
    ofSetColor(ofColor::white);

    if(settings.isEcg){
        ecg.draw();
        network.draw();
    }else{
        mediaController.draw();
        if(settings.isClient){
            ofSetColor(ofColor::black, 150);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        }
        network.draw();
        mediaSequencer.draw();
    }

    if(mediaLoader.hasErrors()){
        mediaLoader.drawErrors();
    }

    ofSetColor(ofColor::white);
    if(settings.isServer && settings.serverDrawingEnabled){
        ofDrawBitmapString("v            toggle audio unit manager\n\nd            toggle server display\n\nD            toggle client display\n\nup/down      select ecg client\n\n-            sync to ecg client", 130, height - 168);
        drawFps(490, height - 60);
        smallFont.drawString(mediaSequencer.getCurrentCollection(), 490, height - 220);
    }

    if((settings.isClient || settings.isEcg) && settings.clientDrawingEnabled){
        ofDrawBitmapString("Computer name: '" + settings.computerName + "'\nDimensions:    " + ofToString(width) + " x " + ofToString(height), width - 350, 40);
        drawFps(320, height - 46);
    }

    if(settings.isClient && settings.clientDrawingEnabled){
        smallFont.drawString("#" + ofToString(network.getClientId()), 50, height - 90);
        ofDrawBitmapString(mediaController.getClientVideoState(), 50, height - 220);
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
