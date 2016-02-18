#include "ofApp.h"

void ofApp::setup(){
    mediaManager.setup();
    network.setup();
    network.setNumChannels(2);
    network.toggleDrawing();
    sequencer.setup(&network, &mediaManager);
}

void ofApp::update(){
    if(network.isRunning()){
        enableFileLogging(network.isRunningServer());
    }else{
        disableFileLogging();
    }
}

void ofApp::draw(){
    
}

void ofApp::keyReleased(int key){
    if(key == 'l'){
        erLog("test log " + ofToString(ofRandom(200)));
    }
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            mediaManager.play(params);
        }
        if(key == '1'){
            params.newPlayCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            mediaManager.play(params);
        }
        if(key == '2'){
            params.newPlayCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            mediaManager.play(params);
        }
    }
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'd'){
        network.toggleDrawing();
    }
}