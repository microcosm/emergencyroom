#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erPlayParams.h"
#include "erLogger.h"

#define ER_VIDEO_LENGTH 120

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _media, erMediaPlayer* _renderer);
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
    erMediaLoader* mediaLoader;
    erMediaPlayer* renderer;
    erPlayParams params;
    int currentChannel, numChannels;
    float speed;
};