#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erMediaQueue.h"
#include "erMediaPlayer.h"
#include "erPlayParams.h"
#include "erLogger.h"
#include "erEcgRenderer.h"
#include "erEcgTimer.h"

#define ER_THEME_LENGTH 1800

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player);
    void setupEcgMode(erNetwork* _network, erMediaPlayer* _player);
    void setupShuffledIndexing();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void messageReceived(string& messageStr);
    string getCurrentCollection();
protected:
    void setSequencerDelay();
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
    erEcgTimer* ecgTimer;

    erMediaQueue queue;
    map<string, erMediaQueue> queues;
    string currentCollection;
    bool queuesLoaded, collectionsLoaded;

    vector<int> shuffledCollectionIndices;
    int currentCollectionIndex;
    int currentSequencerDelay;
    u_int64_t nextTriggerTime;

    int currentChannel;
    float speed;
};