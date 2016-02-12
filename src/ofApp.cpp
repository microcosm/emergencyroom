#include "ofApp.h"

void ofApp::setup(){
    network.setup();
    network.setNumChannels(2);
    network.enableDrawing();
    ofAddListener(network.getClient()->messageReceived, this, &ofApp::onMessageReceived);

    soundPlayer.load("test/audio.mp3");
    soundPlayer.setLoop(false);
    videoPlayer.load("test/video.mov");
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

void ofApp::onMessageReceived(string& message){
    messageParts = ofSplitString(message, " ");
    if(messageParts.size() == 2 && messageParts[0] == TEST_COMMAND && network.isClientReady()){
        time = ofToInt(messageParts[1]);
        time = network.getClientDelay(time);
        soundPlayer.play(time);
        videoPlayer.play(time);
    }
    if(messageParts.size() == 3 && messageParts[0] == PLAY_COMMAND && network.isClientReady()){
        time = ofToInt(messageParts[1]);
        time = network.getClientDelay(time);
        argumentParts = ofSplitString(messageParts[2], ",");
        variableParts = ofSplitString(argumentParts[0], "=");
        speed = ofToFloat(variableParts[1]);
        soundPlayer.setSpeed(speed);
        soundPlayer.play(time);

        videoPlayer.setPosition(0);
        videoPlayer.setSpeed(speed);
        videoPlayer.play(time);
    }
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't' && network.flood(TEST_COMMAND, DEFAULT_DELAY)){
            soundPlayer.play(DEFAULT_DELAY);
            videoPlayer.setPosition(0);
            videoPlayer.play(DEFAULT_DELAY);
        }
        if(key == '1' && network.target(1, PLAY_COMMAND, "speed=0.5", DEFAULT_DELAY)){
            soundPlayer.setSpeed(0.5);
            soundPlayer.play(DEFAULT_DELAY);
            videoPlayer.setPosition(0);
            videoPlayer.setSpeed(0.5);
            videoPlayer.play(DEFAULT_DELAY);
        }
        if(key == '2' && network.target(2, PLAY_COMMAND, "speed=2", DEFAULT_DELAY)){
            soundPlayer.setSpeed(2);
            soundPlayer.play(DEFAULT_DELAY);
            videoPlayer.setPosition(0);
            videoPlayer.setSpeed(0.5);
            videoPlayer.play(DEFAULT_DELAY);
        }
    }
}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){

}
