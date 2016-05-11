#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"
#include "erTranslater.h"
#include "erSettings.h"
#include "erLogger.h"

#define SYNC_TCP_PORT 15001
#define FINDER_TIMEOUT 15000

enum erNetworkRole {
    NETWORK_ROLE_UNDEFINED,
    NETWORK_ROLE_CLIENT,
    NETWORK_ROLE_SERVER
};

class erNetwork{
    
public:
    void setup();
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    virtual void keyReleased(ofKeyEventArgs& args);
    void syncEcg(int delay);
    int getClientId();
    void clientStopAll();
    void clientDisplaysOn();
    void clientDisplaysOff();
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
    void onClientConnectionLost();
    void onClientMessageReceived(string& message);

protected:
    void send(erPlayParams& params, ofxNetworkSyncClientState* client);
    void sendChannelUpdates();
    void setLogLevels(ofLogLevel level);
    void drawBlackOverlay();

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
    erNetworkRole role, previousRole;
    erTranslater translater;

    ofTrueTypeFont font;
    string statusText, clientChannel;
    bool serverRequested, serverIsAllowed;
    int numClients, previousNumClients;
    int finderStartTime, serverPortOffset;
    bool success;
    u_int64_t now;
    int ecgIndex;
};
