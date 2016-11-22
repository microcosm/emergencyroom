#include "erLogger.h"

string erProgramId;
bool erFileLoggingEnabled = false;

void erEnableFileLogging(bool isServer){
    if(!erFileLoggingEnabled){
        erProgramId = ofToString(ofRandom(1000, 9999));
        erFileLoggingEnabled = true;
        string clientServer = isServer ? "server" : "client";
        ofLogToFile("log/" + clientServer + "-" + erProgramId + ".txt");
    }
}

void erDisableFileLogging(){
    if(erFileLoggingEnabled){
        erFileLoggingEnabled = false;
        ofLogToConsole();
    }
}

void erLog(string methodSignature, string message){
    ofLog(OF_LOG_NOTICE, ofGetTimestampString() + ": " + methodSignature + " " + message);
    cout.flush();
}
