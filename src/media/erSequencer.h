#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMedia.h"
#include "erPlayParams.h"
#include "erLogger.h"

#define ER_COLLECTION_LENGTH 2400 //~40 secs
#define ER_VIDEO_LENGTH 480 //~8 secs

class erSequencer{

public:
    void setup(erNetwork* _network, erMedia* _media);
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    void playNewVideo();
    string chooseVideo();
    void incrementCurrentChannel();
    erNetwork* network;
    erTranslater* translater;
    erMedia* media;
    erPlayParams params;
    int currentChannel, numChannels;
    float speed;
};