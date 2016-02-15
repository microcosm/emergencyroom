#include "ofApp.h"

void ofApp::setup(){
    network.setup();
    network.setNumChannels(2);
    network.enableDrawing();
    translater = network.getTranslater();
    ofAddListener(network.getClient()->messageReceived, this, &ofApp::messageReceived);

    soundPlayer.load("test/audio.mp3");
    soundPlayer.setLoop(false);
    videoPlayer.load("test/fingers.mov");
    videoPlayer.setLoopState(OF_LOOP_NONE);
}

void ofApp::update(){
    videoPlayer.update();
}

void ofApp::draw(){
    if(videoPlayer.isLoaded()){
        videoPlayer.draw(10, 120);
    }
}

void ofApp::play(float speed, int delay){
    soundPlayer.setSpeed(speed);
    soundPlayer.schedule(delay);
    videoPlayer.setSpeed(speed);
    videoPlayer.schedule(delay);
}

void ofApp::play(){
    soundPlayer.execute(params);
    videoPlayer.execute(params);
}

void ofApp::messageReceived(string& message){
    params = translater->toParams(message);
    if(params.isTestCommand() || params.isPlayCommand()) play();
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            play();
        }
        if(key == '1'){
            params.newPlayCommand();
            params.setSpeed(0.5);
            network.target(1, params);
            play();
        }
        if(key == '2'){
            params.newPlayCommand();
            params.setSpeed(2);
            network.target(2, params);
            play();
        }
    }
}