#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "erMediaSequencer.h"
#include "erMediaController.h"

class erMediaSequencer;
class erMediaController;

class erStatusRenderer{

public:
    void setup(erNetwork* _network, erMediaSequencer* _mediaSequencer, erMediaController* _mediaController);
    void draw();
    void drawFps(int x, int y);
    void drawOverlay();

protected:
    erNetwork* network;
    ofTrueTypeFont bigFont, smallFont;
    int width, height;
    erMediaSequencer* mediaSequencer;
    erMediaController* mediaController;
};
