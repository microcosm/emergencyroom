#pragma once
#include "ofMain.h"
#include "erMedia.h"
#include "erNetwork.h"

class erMediaRenderer{

public:
    void setup(erMedia* _media, erNetwork* _network, int _numChannels);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    
protected:
    void drawClient();
    void drawServer();
    erMedia* media;
    erNetwork* network;
    int numChannels;
};