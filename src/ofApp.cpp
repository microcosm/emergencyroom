#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    ofSetWindowShape(1200, 800);
    settings.load();
    if(settings.fullscreenByDefault){
        ofToggleFullscreen();
    }

    player.setTestSoundPlayer(&loader.testSoundPlayer);
    player.setTestVideoPlayer(&loader.testVideoPlayer);
    player.setVideoPlayers(&loader.videoPlayers);

    if(settings.isEcg){
        player.setupEcgMode(&network);
        network.setup();
        sequencer.setupEcgMode(&network, &player);
    }else{
        loader.setup(&network);
        player.setup(&network);
        network.setup();
        sequencer.setup(&network, &loader, &player);
        ofAddListener(ofEvents().draw, this, &ofApp::draw);
    }
}

void ofApp::update(){
    if(network.isRunning() && settings.logToFileEnabled){
        erEnableFileLogging(network.isRunningServer());
    }else{
        erDisableFileLogging();
    }

    if(ofGetFrameNum() > 0){
        settings.isServer ? network.requestServer() : network.denyServer();
    }
}

void ofApp::draw(ofEventArgs& args){
    if(settings.drawingEnabled){
        if(network.isRunningServer()){
            ofSetColor(ofColor::white);
            ofDrawBitmapString("v            toggle audio unit manager\n\nd            toggle server display\n\nup/down      select ecg client\n\n-            sync to ecg client", 130, ofGetHeight() - 130);
        }

        if(loader.hasErrors()){
            loader.drawErrors();
        }
    }
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            player.playTest(params);
        }
        if(key == '1'){
            params.newVideoCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '2'){
            params.newVideoCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '9'){
            params.newGraphicCommand();
            params.setPath("test");
            network.target(1, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
    }else{
        if(key == 's'){
            network.requestServer();
        }
    }
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'd'){
        settings.toggleDrawing();
    }
}