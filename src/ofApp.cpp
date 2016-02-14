#include "ofApp.h"

void ofApp::setup(){
    network.setup();
    network.setNumChannels(2);
    network.enableDrawing();
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

void ofApp::play(int delay, float speed){
    soundPlayer.setSpeed(speed);
    soundPlayer.schedule(delay);
    videoPlayer.setSpeed(speed);
    videoPlayer.schedule(delay);
}

void ofApp::messageReceived(string& message){
    messageParts = ofSplitString(message, " ");
    if(messageParts.size() == 2 && messageParts[0] == TEST_COMMAND && network.isClientReady()){
        delay = ofToInt(messageParts[1]);
        delay = network.getClientDelay(delay);
        play(delay, 1);
    }
    if(messageParts.size() == 3 && messageParts[0] == PLAY_COMMAND && network.isClientReady()){
        delay = ofToInt(messageParts[1]);
        delay = network.getClientDelay(delay);
        argumentParts = ofSplitString(messageParts[2], ",");
        variableParts = ofSplitString(argumentParts[0], "=");
        speed = ofToFloat(variableParts[1]);
        play(delay, speed);
    }
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't' && network.flood(TEST_COMMAND, DEFAULT_DELAY)){
            play(DEFAULT_DELAY, 1);
        }
        if(key == '1' && network.target(1, PLAY_COMMAND, "speed=0.5", DEFAULT_DELAY)){
            play(DEFAULT_DELAY, 0.5);
        }
        if(key == '2' && network.target(2, PLAY_COMMAND, "speed=2", DEFAULT_DELAY)){
            play(DEFAULT_DELAY, 2);
        }
    }
}