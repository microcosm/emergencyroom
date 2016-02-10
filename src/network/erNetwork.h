#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "ofSoundPlayerDelayed.h"

#define SYNC_TCP_PORT 12345
#define FINDER_TIMEOUT 10000
#define SOUND_PLAYER_DELAY 1000
#define SOUND_PLAYER_INTERVAL 2000

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
    void playTestSound();
    bool isRunningClient();
    bool isRunningServer();
    void keyPressed(int key);
    void onMessageReceived(string & message);
    void onServerFound(IpAndPort & info);
    void onConnectionLost();

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    ofSoundPlayerDelayed player;

    erNetworkRole role;
    int finderStartTime;
    string statusText;
    int serverPortOffset;
    bool drawingEnabled;
    unsigned long long now;
};
