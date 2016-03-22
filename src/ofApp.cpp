#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(420, 300);

    media.setup();
    renderer.setup(&network, NUM_CHANNELS);
    renderer.setTestSoundPlayer(&media.testSoundPlayer);
    renderer.setTestVideoPlayer(&media.testVideoPlayer);
    renderer.setVideoPlayers(&media.videoPlayers);
    network.setup(NUM_CHANNELS);
    sequencer.setup(&network, &media);

    network.toggleDrawing();
}

void ofApp::update(){
    if(network.isRunning()){
        erEnableFileLogging(network.isRunningServer());
    }else{
        erDisableFileLogging();
    }
}

void ofApp::draw(){
    
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            media.play(params);
        }
        if(key == '1'){
            params.newVideoCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            media.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '2'){
            params.newVideoCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            media.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '9'){
            params.newGraphicCommand();
            params.setPath("test");
            network.target(1, params);
            media.play(params);
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
        network.toggleDrawing();
    }
}