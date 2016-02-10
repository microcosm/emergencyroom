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
    const string messagePlay = "PLAY ";
    if(message.find(messagePlay) == 0 && network.isClientReady()){
        time = ofToInt(message.substr(messagePlay.length()));
        player.play(network.getClientDelay(time));
    }
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(network.broadcast("PLAY ", SOUND_PLAYER_DELAY)){
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
