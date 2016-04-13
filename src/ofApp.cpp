#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(420, 300);
    settings.load();
    if(settings.fullscreenByDefault){
        ofToggleFullscreen();
    }

    loader.setup(&network);
    loader.setMasterVolume(settings.masterVolume);
    player.setup(&network, NUM_CHANNELS);
    player.setTestSoundPlayer(&loader.testSoundPlayer);
    player.setTestVideoPlayer(&loader.testVideoPlayer);
    player.setVideoPlayers(&loader.videoPlayers);
    network.setup(NUM_CHANNELS);
    sequencer.setup(&network, &loader, &player);

    network.toggleDrawing();
    ofAddListener(ofEvents().draw, this, &ofApp::draw);
}

void ofApp::update(){
    if(network.isRunning()){
        erEnableFileLogging(network.isRunningServer());
    }else{
        erDisableFileLogging();
    }
}

void ofApp::draw(ofEventArgs& args){
    if(loader.hasErrors()){
        loader.drawErrors();
    }
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            player.play(params);
        }
        if(key == '1'){
            params.newVideoCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            player.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '2'){
            params.newVideoCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            player.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '9'){
            params.newGraphicCommand();
            params.setPath("test");
            network.target(1, params);
            player.play(params);
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