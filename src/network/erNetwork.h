#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"

#define SYNC_TCP_PORT 12345
#define FINDER_TIMEOUT 10000

enum erNetworkRole {
    NETWORK_ROLE_UNDEFINED,
    NETWORK_ROLE_CLIENT,
    NETWORK_ROLE_SERVER
};

class erNetwork{
    
public:
    void setup(int _defaultDelay);
    void setNumChannels(int _numChannels);
    virtual void update(ofEventArgs& updateArgs);
    virtual void draw(ofEventArgs& updateArgs);
    void enableDrawing();
    bool flood(string command);
    bool flood(string command, int delay);
    bool target(int target, string command, string arguments);
    bool target(int target, string command, string arguments, int delay);
    bool isRunningClient();
    bool isRunningServer();
    bool isClientReady();
    int getClientDelay(int serverDelay);
    ofxNetworkSyncClient* getClient();
    void keyPressed(int key);
    void onServerFound(IpAndPort& info);
    void onConnectionLost();

protected:
    void send(string message, ofxNetworkSyncClientState* client);
    string format(string command, int delay, string arguments="");
    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    erNetworkRole role;

    string statusText;
    int defaultDelay, finderStartTime, serverPortOffset, numChannels;
    bool drawingEnabled, success;
    unsigned long long now;
};
