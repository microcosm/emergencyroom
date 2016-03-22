#pragma once
#include "ofMain.h"
#include "erMedia.h"
#include "erNetwork.h"
#define SCREEN_MARGIN 20

class erMediaRenderer{

public:
    void setup(erMedia* _media, erNetwork* _network, int _numChannels);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    
protected:
    void calculatePreviewSize();
    void drawClient();
    void drawServer();
    erMedia* media;
    erNetwork* network;
    int numChannels, width, height;
};