#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"
#include "erTranslater.h"

#define SYNC_TCP_PORT 15001
#define FINDER_TIMEOUT 15000

enum erNetworkRole {
    NETWORK_ROLE_UNDEFINED,
    NETWORK_ROLE_CLIENT,
    NETWORK_ROLE_SERVER
};

class erNetwork{
    
public:
    void setup(int _numChannels);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    virtual void keyReleased(ofKeyEventArgs& args);
    void toggleDrawing();
    void requestServer();
    bool flood(erPlayParams params);
    bool target(int target, erPlayParams params);
    bool wasRunningClient();
    bool wasRunningServer();
    bool isRunningClient();
    bool isRunningServer();
    bool isRunning();
    bool justBecameClient();
    bool justBecameServer();
    void denyServer();
    ofEvent<string>& clientMessageReceived();
    erTranslater* getTranslater();
    int getNumChannels();
    void onClientConnectionLost();
    void onClientMessageReceived(string& message);

protected:
    void send(erPlayParams& params, ofxNetworkSyncClientState* client);
    void sendChannelUpdates();
    void setLogLevels(ofLogLevel level);

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    erNetworkRole role, previousRole;
    erTranslater translater;

    string statusText, clientChannel;
    bool serverRequested, serverIsAllowed;
    int numClients, previousNumClients;
    int finderStartTime, serverPortOffset, numChannels;
    bool drawingEnabled, success;
    unsigned long long now;
    int ecgIndex;
};
