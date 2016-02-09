#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "ofSoundPlayerDelayed.h"

#define SYNC_TCP_PORT 12345
#define FINDER_TIMEOUT 10000
#define SOUND_PLAYER_DELAY 1000
#define SOUND_PLAYER_INTERVAL 10000

class erNetwork{
    
public:
    void setup();
    virtual void update(ofEventArgs& updateArgs);
    virtual void draw(ofEventArgs& updateArgs);
    void enableDrawing();
    void keyPressed(int key);
    void onMessageReceived(string & message);
    void onServerFound(IpAndPort & info);
    void onClientConnectionLost();

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    ofSoundPlayerDelayed player;

    int lastUpdateTime, finderStartTime;

    string statusText;
    int serverPortOffset;
    bool drawingEnabled;
    unsigned long long now;
};
