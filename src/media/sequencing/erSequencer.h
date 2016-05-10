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
#include "ofxAnimatable.h"

#define ER_THEME_LENGTH 1800

class erSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _loader, erMediaPlayer* _player);
    void setupEcgMode(erNetwork* _network, erMediaPlayer* _player);
    void setupShuffledIndexing();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    void stopAll();
    void messageReceived(string& messageStr);
    string getCurrentCollection();
    void ecgBpmLooped(ofxAnimatable::AnimationEvent& args);
protected:
    void setSequencerDelay();
    void playNewVideo();
    void chooseNewTheme();
    int chooseNewChannel();
    void loadChannels();
    void attemptToLoadCollections();
    void attemptToLoadMediaQueues();
    bool isAudioPlaying();
    void prepareParams(string path, int speed);

    erNetwork* network;
    erTranslater* translater;
    erMediaLoader* loader;
    erMediaPlayer* player;
    erPlayParams params;
    erEcgRenderer ecg;
    erEcgTimer* ecgTimer;

    erMediaQueue queue;
    map<string, erMediaQueue> queues;
    string currentCollection, focusVideoPath;
    bool queuesLoaded, collectionsLoaded, ecgTimerStarted, focusTime;

    vector<int> shuffledCollectionIndices, shuffledChannels;
    int currentCollectionIndex, currentSequencerDelay, currentChannelIndex, currentChannel, focusIndex;
    u_int64_t nextTriggerTime;
    float speed;
};