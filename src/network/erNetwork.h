#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"

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
    int getClientDelay(int serverDelay);
    ofxNetworkSyncClient* getClient();
    void keyPressed(int key);
    void onServerFound(IpAndPort& info);
    void onConnectionLost();
    erPlayParams getPlayParams(string& messageStr);

protected:
    void send(string message, ofxNetworkSyncClientState* client);
    string format(string command, int delay, string arguments="");
    void setLogLevels(ofLogLevel level);

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    erNetworkRole role;

    string statusText;
    int finderStartTime, serverPortOffset, numChannels;
    bool drawingEnabled, success;
    unsigned long long now;
    vector<string> messageParts, argumentParts, variableParts;
};
