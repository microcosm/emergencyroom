#pragma once
#include "ofMain.h"
#include "erLogger.h"
#include "erSettings.h"
#include "erMediaSequencer.h"
#include "erStatusRenderer.h"
#include "erMediaController.h"
#include "erClientController.h"
#include "erOmxManager.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyReleased(int key);
    void toggleFullscreen();
    void exit();

protected:
    erMediaLoader mediaLoader;
    erMediaController mediaController;
    erMediaSequencer mediaSequencer;
    erNetwork network;
    erPlayParams params;
    erClientController clientController;
    erEcgTimer ecgTimer;
    erStatusRenderer statusRenderer;
    erOmxManager omxManager;
    bool videoSoundAssigned;
};
