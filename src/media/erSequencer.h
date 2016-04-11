#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erPlayParams.h"
#include "erLogger.h"

#define ER_VIDEO_LENGTH 120

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player);
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    void playNewVideo();
    string chooseVideo();
    string chooseRandom(vector<string>* videos);
    bool isAudioPlaying();
    int incrementCurrentChannel();
    erNetwork* network;
    erTranslater* translater;
    erMediaLoader* loader;
    erMediaPlayer* player;
    erPlayParams params;
    int currentChannel, numChannels;
    float speed;
};