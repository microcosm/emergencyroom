#include "ofApp.h"

void ofApp::setup(){
    mediaManager.setup();
    network.setup();
    network.setNumChannels(2);
    network.toggleDrawing();
    translater = network.getTranslater();
    ofAddListener(network.clientMessageReceived(), this, &ofApp::messageReceived);
}

void ofApp::update(){
    
}

void ofApp::draw(){
    
}

void ofApp::messageReceived(string& message){
    params = translater->toParams(message);
    if(params.isPlayable()){
        mediaManager.play(params);
    }
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            mediaManager.play(params);
        }
        if(key == '1'){
            params.newPlayCommand();
            params.setSpeed(0.5);
            network.target(1, params);
            mediaManager.play(params);
        }
        if(key == '2'){
            params.newPlayCommand();
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