#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"
#include "erTranslater.h"

#define SYNC_TCP_PORT 12345
#define FINDER_TIMEOUT 10000

enum erNetworkRole {
    NETWORK_ROLE_UNDEFINED,
    NETWORK_ROLE_CLIENT,
    NETWORK_ROLE_SERVER
};

class erNetwork{
    
public:
    void setup();
    void setNumChannels(int _numChannels);
    virtual void update(ofEventArgs& updateArgs);
    virtual void draw(ofEventArgs& updateArgs);
    void enableDrawing();
    bool flood(erPlayParams params);
    bool target(int target, erPlayParams params);
    bool isRunningClient();
    bool isRunningServer();
    ofEvent<string>& clientMessageReceived();
    erTranslater* getTranslater();
    void onConnectionLost();

protected:
    void send(erPlayParams& params, ofxNetworkSyncClientState* client);
    void setLogLevels(ofLogLevel level);

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    erNetworkRole role;
    erTranslater translater;

    string statusText;
    int finderStartTime, serverPortOffset, numChannels;
    bool drawingEnabled, success;
    unsigned long long now;
};
