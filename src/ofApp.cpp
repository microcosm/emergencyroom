#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(1200, 800);
    bigFont.load(ER_FONT_PATH, 100);
    smallFont.load(ER_FONT_PATH, 50);
    videoSoundAssigned = false;
    settings.load();
    if(settings.fullscreenByDefault){
        ofToggleFullscreen();
    }

    player.setVideoPaths(&loader.allVideos);
    player.setVideoPlayers(&loader.videoPlayers);
    player.setTexts(&loader.texts);

    if(settings.isEcg){
        player.setupEcgMode(&network);
        network.setup();
        ecg.setup(&network);
    }else{
        loader.setup(&network);
        player.setup(&network);
        network.setup();
        sequencer.setup(&network, &loader, &player);
    }
}

void ofApp::update(){
    width = ofGetWidth();
    height = ofGetHeight();

    if(loader.isLoaded() && !videoSoundAssigned){
        player.useSoundRendererFor(loader.audibleVideos);
        videoSoundAssigned = true;
    }

    if(network.isRunning() && settings.logToFileEnabled){
        erEnableFileLogging(network.isRunningServer());
    }else{
        erDisableFileLogging();
    }

    if(ofGetFrameNum() > 0){
        settings.isServer ? network.requestServer() : network.denyServer();
    }

    if(settings.isEcg){
        ecg.update();
        network.update();
    }else{
        loader.update();
        player.update();
        network.update();
        sequencer.update();
    }
}

void ofApp::draw(){
    ofSetColor(ofColor::white);

    if(settings.isEcg){
        ecg.draw();
        network.draw();
    }else{
        player.draw();
        network.draw();
        sequencer.draw();
    }

    if(loader.hasErrors()){
        loader.drawErrors();
    }

    ofSetColor(ofColor::white);
    if(network.isRunningServer() && settings.serverDrawingEnabled){
        ofDrawBitmapString("v            toggle audio unit manager\n\nd            toggle server display\n\nD            toggle client display\n\nup/down      select ecg client\n\n-            sync to ecg client", 130, height - 168);
        drawFps(490, height - 60);
        smallFont.drawString(sequencer.getCurrentCollection(), 490, height - 220);
    }

    if((network.isRunningClient() || settings.isEcg) && settings.clientDrawingEnabled){
        ofDrawBitmapString("Computer name: '" + settings.computerName + "'\nDimensions:    " + ofToString(width) + " x " + ofToString(height), width - 350, 40);
        drawFps(320, height - 46);
    }

    if(network.isRunningClient() && settings.clientDrawingEnabled){
        smallFont.drawString("#" + ofToString(network.getClientId()), 50, height - 90);
        ofDrawBitmapString(player.getClientVideoState(), 50, height - 220);
    }
}

void ofApp::drawFps(int x, int y){
    bigFont.drawString("fps: " + ofToString(int(ofGetFrameRate())), x, y);
}

void ofApp::keyReleased(int key){
    if(!network.isRunningServer() && key == 's'){
        network.requestServer();
    }
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
        sequencer.stopAll();
    }
}