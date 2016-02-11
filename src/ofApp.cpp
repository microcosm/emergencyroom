#include "ofApp.h"

void ofApp::setup(){
    network.setup();
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
    parts = ofSplitString(message, " ");
    if(parts.size() == 2 && parts[0] == TEST_COMMAND && network.isClientReady()){
        time = ofToInt(parts[1]);
        player.play(network.getClientDelay(time));
    }
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't' && network.broadcast(TEST_COMMAND, SOUND_PLAYER_DELAY)){
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
