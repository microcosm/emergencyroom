#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(420, 300);

    mediaLoader.setup(&network);
    missingVideos = mediaLoader.hasMissingVideos();
    renderer.setup(&network, NUM_CHANNELS);
    renderer.setTestSoundPlayer(&mediaLoader.testSoundPlayer);
    renderer.setTestVideoPlayer(&mediaLoader.testVideoPlayer);
    renderer.setVideoPlayers(&mediaLoader.videoPlayers);
    network.setup(NUM_CHANNELS);
    sequencer.setup(&network, &mediaLoader, &renderer);

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
    if(missingVideos){
        ofSetColor(ofColor::black);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);
        ofDrawBitmapString("Missing from live folder", 20, 30);
        int i = 1;
        for(const auto video : mediaLoader.missingVideos){
            ofDrawBitmapString(video, 20, 30 + 20 * i++);
        }
    }
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            renderer.play(params);
        }
        if(key == '1'){
            params.newVideoCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            renderer.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '2'){
            params.newVideoCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            renderer.play(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '9'){
            params.newGraphicCommand();
            params.setPath("test");
            network.target(1, params);
            renderer.play(params);
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