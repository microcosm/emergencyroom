#include "erOmxManager.h"

void erOmxManager::setup(){
#ifdef __linux__
    currentPlayer = 0;

    ofxOMXPlayerSettings omxSettings;
    omxSettings.enableLooping = false;
    omxSettings.enableAudio = false;
    omxSettings.useHDMIForAudio = false;
    omxSettings.enableTexture = true;

    omxPlayer1.setup(omxSettings);
    omxPlayer2.setup(omxSettings);

    omxPlayer1.setPaused(true);
    omxPlayer2.setPaused(true);
#endif
}

void erOmxManager::update(){
#ifdef __linux__

#endif
}

void erOmxManager::draw(float x, float y, float width, float height){
#ifdef __linux__
    ofBackground(ofColor::red);
    ofSetBackgroundColor(ofColor::green);
    ofSetColor(ofColor::white);
    activePlayer()->draw(x, y, width, height);
#endif
}

void erOmxManager::prepare(string absolutePath){
#ifdef __linux__
    inactivePlayer()->loadMovie(absolutePath);
    inactivePlayer()->setPaused(true);
#else
    throw invalid_argument("This path should never be reached.");
#endif
}

void erOmxManager::begin(){
#ifdef __linux__
    inactivePlayer()->restartMovie();
    inactivePlayer()->setPaused(false);
    switchActivePlayers();
#else
    throw invalid_argument("This path should never be reached.");
#endif
}

float erOmxManager::getDuration(string absolutePath){
#ifdef __linux__
    prepare(absolutePath);
    return inactivePlayer()->getDurationInSeconds();
#else
    throw invalid_argument("This path should never be reached.");
#endif
}

bool erOmxManager::isActiveMoviePlaying(){
#ifdef __linux__
    return activePlayer()->isPlaying();
#else
    throw invalid_argument("This path should never be reached.");
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