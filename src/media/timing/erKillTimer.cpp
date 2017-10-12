#include "erKillTimer.h"

void erKillTimer::setup(){
    registerMessageRecieved();
}

void erKillTimer::update(){
    if(ofGetElapsedTimeMillis() > lastMessageRecievedTime + ER_TIMEOUT_MS){
        string msg = "Timeout occurred. Ellapsed time = " + ofToString(ofGetElapsedTimeMillis()) + ", last message reciept time = " + ofToString(lastMessageRecievedTime) + ". Closing application.";
        erLog("erKillTimer::update()", msg);
        OF_EXIT_APP(0);
    }
}

void erKillTimer::registerMessageRecieved(){
    lastMessageRecievedTime = ofGetElapsedTimeMillis();
}
