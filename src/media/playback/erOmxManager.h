#pragma once
#include "ofMain.h"
#include "erUtils.h"

#ifdef __linux__
#include "ofxOMXPlayer.h"
#endif

class erOmxManager{

public:
    void setup();
    void update();
    void draw(float x, float y, float width, float height);

    void prepare(string absolutePath);
    void begin();

    float getDuration(string absolutePath);
    bool isActiveMoviePlaying();

#ifdef __linux__
protected:
    void switchActivePlayers();
    ofxOMXPlayer* activePlayer();
    ofxOMXPlayer* inactivePlayer();

    ofxOMXPlayer omxPlayer1;
    ofxOMXPlayer omxPlayer2;
    int currentPlayer;
    ofTexture texture;
#endif
};
