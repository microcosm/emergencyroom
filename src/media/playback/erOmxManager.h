#pragma once
#include "ofMain.h"
#include "erUtils.h"

#ifdef __linux__
#include "erOmxPlayer.h"
#endif

class erOmxManager{

public:
    void setup(string sampleMoviePath);
    void update();
    void draw(float x, float y, float width, float height);

    void prepare(string absolutePath);
    void begin();

    float getDuration(string absolutePath);
    bool isActiveMoviePlaying();

#ifdef __linux__
protected:
    void switchActivePlayers();
    erOmxPlayer* activePlayer();
    erOmxPlayer* inactivePlayer();

    erOmxPlayer omxPlayer1;
    erOmxPlayer omxPlayer2;
    int currentPlayer;
    ofTexture texture;
    bool hasSetup = false;
#endif
};
