#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erMediaLoader.h"
#include "erMediaQueue.h"
#include "erMediaController.h"
#include "erPlayParams.h"
#include "erLogger.h"
#include "erEcgTimer.h"
#include "ofxAnimatable.h"

#define ER_THEME_LENGTH 1800

class erMediaSequencer{

public:
    void setup(erNetwork* _network, erMediaLoader* _mediaLoader, erMediaController* _mediaController, erEcgTimer* _ecgTimer);
    void setupShuffledIndexing();
    void update();
    void draw();
    void stopAll();
    void messageReceived(string& messageStr);
    string getCurrentCollection();
    void ecgBpmLooped(ofxAnimatable::AnimationEvent& args);
protected:
    void handleBpmLooped();
    void handleStopAll();
    void handleMessageReceived();
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
    erMediaLoader* mediaLoader;
    erMediaController* mediaController;
    erPlayParams params;
    erEcgTimer* ecgTimer;

    erMediaQueue queue;
    map<string, erMediaQueue> queues;
    string currentCollection, focusVideoPath;
    bool queuesLoaded, collectionsLoaded, focusTime;

    vector<int> shuffledCollectionIndices, shuffledChannels;
    int currentCollectionIndex, currentSequencerDelay, currentChannelIndex, currentChannel, focusIndex;
    u_int64_t nextTriggerTime;
    float speed;
    bool bpmLooped = false;
    bool stopAllReceived = false;
    bool aMessageReceived = false;
    string messageContent;
};
