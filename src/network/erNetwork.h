#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "ofSoundPlayerDelayed.h"

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
    virtual void update(ofEventArgs& updateArgs);
    virtual void draw(ofEventArgs& updateArgs);
    void enableDrawing();
    bool broadcast(string command, int delay);
    bool isRunningClient();
    bool isRunningServer();
    bool isClientReady();
    int getClientDelay(int serverDelay);
    ofxNetworkSyncClient* getClient();
    void keyPressed(int key);
    void onServerFound(IpAndPort& info);
    void onConnectionLost();

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;

    erNetworkRole role;
    int finderStartTime;
    string statusText;
    int serverPortOffset;
    bool drawingEnabled, broadcastSuccessful;
    unsigned long long now;
};
