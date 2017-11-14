#include "erOmxManager.h"

void erOmxManager::setup(string sampleMoviePath){
#ifdef __linux__
    currentPlayer = 0;

    ofxOMXPlayerSettings omxSettings;
    omxSettings.videoPath = sampleMoviePath;
    omxSettings.enableLooping = false;
    omxSettings.enableAudio = false;
    omxSettings.useHDMIForAudio = false;
    omxSettings.enableTexture = true;

    omxPlayer1.setup(omxSettings);
    omxPlayer2.setup(omxSettings);

    omxPlayer1.setPaused(true);
    omxPlayer2.setPaused(true);

    hasSetup = true;
#endif
}

void erOmxManager::update(){
#ifdef __linux__

#endif
}

void erOmxManager::draw(float x, float y, float width, float height){
#ifdef __linux__
    ofSetColor(ofColor::white);
    activePlayer()->draw(x, y, width, height);
#endif
}

void erOmxManager::prepare(string absolutePath){
#ifdef __linux__
    if(!hasSetup){
        setup(absolutePath);
    }
    inactivePlayer()->loadMovie(absolutePath);
    inactivePlayer()->setPaused(true);
#else
    erThrowInvalidPath("erOmxManager::prepare(string absolutePath)");
#endif
}

void erOmxManager::begin(){
#ifdef __linux__
    inactivePlayer()->restartMovie();
    inactivePlayer()->setPaused(false);
    switchActivePlayers();
#else
    erThrowInvalidPath("erOmxManager::begin()");
#endif
}

float erOmxManager::getDuration(string absolutePath){
#ifdef __linux__
    prepare(absolutePath);
    return inactivePlayer()->getDurationInSeconds();
#else
    erThrowInvalidPath("erOmxManager::getDuration(string absolutePath)");
#endif
}

bool erOmxManager::isActiveMoviePlaying(){
#ifdef __linux__
    return activePlayer()->isPlaying();
#else
    erThrowInvalidPath("erOmxManager::isActiveMoviePlaying()");
#endif
}

#ifdef __linux__
void erOmxManager::switchActivePlayers(){
    currentPlayer = currentPlayer == 0 ? 1 : 0;
}

ofxOMXPlayer* erOmxManager::activePlayer(){
    return currentPlayer == 0 ? &omxPlayer1 : &omxPlayer2;
}

ofxOMXPlayer* erOmxManager::inactivePlayer(){
    return currentPlayer == 0 ? &omxPlayer2 : &omxPlayer1;
}
#endif
