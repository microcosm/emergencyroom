#include "ofApp.h"

void ofApp::setup(){
    network.setup();
    network.setNumChannels(2);
    network.enableDrawing();
    ofAddListener(network.getClient()->messageReceived, this, &ofApp::onMessageReceived);

    player.load("test-sound.mp3");
    player.setLoop(false);
}

void ofApp::update(){

}

void ofApp::draw(){

}

void ofApp::onMessageReceived(string& message){
    messageParts = ofSplitString(message, " ");
    if(messageParts.size() == 2 && messageParts[0] == TEST_COMMAND && network.isClientReady()){
        time = ofToInt(messageParts[1]);
        player.play(network.getClientDelay(time));
    }
    if(messageParts.size() == 3 && messageParts[0] == PLAY_COMMAND && network.isClientReady()){
        time = ofToInt(messageParts[1]);
        argumentParts = ofSplitString(messageParts[2], ",");
        variableParts = ofSplitString(argumentParts[0], "=");
        player.setSpeed(ofToFloat(variableParts[1]));
        player.play(network.getClientDelay(time));
    }
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't' && network.flood(TEST_COMMAND, SOUND_PLAYER_DELAY)){
            player.play(SOUND_PLAYER_DELAY);
        }
        if(key == '1' && network.target(1, PLAY_COMMAND, "speed=0.5", SOUND_PLAYER_DELAY)){
            player.setSpeed(0.5);
            player.play(SOUND_PLAYER_DELAY);
        }
        if(key == '2' && network.target(2, PLAY_COMMAND, "speed=2", SOUND_PLAYER_DELAY)){
            player.setSpeed(2);
            player.play(SOUND_PLAYER_DELAY);
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
