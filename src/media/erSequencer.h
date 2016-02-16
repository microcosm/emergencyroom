#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaManager.h"
#include "erPlayParams.h"

#define ER_COLLECTION_LENGTH 2400 //~40 secs
#define ER_VIDEO_LENGTH 480 //~8 secs

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaManager* _mediaManager);
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    string selectCollection();
    erNetwork* network;
    erTranslater* translater;
    erMediaManager* mediaManager;
    erPlayParams params;
    int delay, numCollections;
    float speed;
    vector<string> channelsToCollections, videoCollections;
};