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

void ofApp::play(int delay, float speed){
    soundPlayer.setSpeed(speed);
    soundPlayer.play(delay);
    videoPlayer.stop();
    videoPlayer.setSpeed(speed);
    videoPlayer.setPosition(0);
    videoPlayer.play(delay);
}

void ofApp::onMessageReceived(string& message){
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

void ofApp::keyPressed(int key){

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
