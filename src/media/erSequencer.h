#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erMediaPlayer.h"
#include "erPlayParams.h"
#include "erLogger.h"
#include "erEcgVisualization.h"

#define ER_VIDEO_LENGTH 120

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player);
    void setupEcgMode(erNetwork* _network, erMediaPlayer* _player);
    void setupShuffledIndexing();
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    void playNewVideo();
    string getNextSilent();
    string getNextAudible();
    string chooseVideo();
    string chooseRandom(vector<string>* videos);
    bool isAudioPlaying();
    int incrementCurrentChannel();

    erNetwork* network;
    erTranslater* translater;
    erMediaLoader* loader;
    erMediaPlayer* player;
    erPlayParams params;
    erEcgVisualization ecg;
    vector<int> shuffledAudibleVideoIndices, shuffledSilentVideoIndices;
    int currentAudibleVideoIndex, currentSilentVideoIndex;
    bool shuffledIndexingSetup;

    int currentChannel;
    float speed;
};