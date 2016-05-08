#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erMediaQueue.h"
#include "erMediaPlayer.h"
#include "erPlayParams.h"
#include "erLogger.h"
#include "erEcgRenderer.h"

#define ER_VIDEO_LENGTH 120
#define ER_THEME_LENGTH 1800

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player);
    void setupEcgMode(erNetwork* _network, erMediaPlayer* _player);
    void setupShuffledIndexing();
    virtual void update(ofEventArgs& updateArgs);
    void messageReceived(string& messageStr);
protected:
    void playNewVideo();
    void chooseNewTheme();
    void attemptToLoadCollections();
    void attemptToLoadMediaQueues();
    bool isAudioPlaying();
    int incrementCurrentChannel();

    erNetwork* network;
    erTranslater* translater;
    erMediaLoader* loader;
    erMediaPlayer* player;
    erPlayParams params;
    erEcgRenderer ecg;

    erMediaQueue queue;
    map<string, erMediaQueue> queues;
    string currentCollection;
    bool queuesLoaded, collectionsLoaded;

    vector<int> shuffledCollectionIndices;
    int currentCollectionIndex;

    int currentChannel;
    float speed;
};