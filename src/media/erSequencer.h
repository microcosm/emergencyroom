#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erPlayParams.h"
#include "erLogger.h"

#define ER_COLLECTION_LENGTH 2400 //~40 secs
#define ER_VIDEO_LENGTH 480 //~8 secs

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _media, erMediaRenderer* _renderer);
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    void playNewVideo();
    string chooseVideo();
    void incrementCurrentChannel();
    erNetwork* network;
    erTranslater* translater;
    erMediaLoader* mediaLoader;
    erMediaRenderer* renderer;
    erPlayParams params;
    int currentChannel, numChannels;
    float speed;
};