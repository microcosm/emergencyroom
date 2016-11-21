#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"
#include "erTranslater.h"
#include "erSettings.h"
#include "erLogger.h"

class erNetwork{
    
public:
    void setup();
    void update();
    void draw();
    virtual void keyReleased(ofKeyEventArgs& args);
    void syncEcg(int delay);
    int getClientId();
    void clientStopAll();
    void clientDisplaysOn();
    void clientDisplaysOff();
    void requestServer();
    bool flood(erPlayParams params);
    bool target(int target, erPlayParams params);
    bool isRunningClient();
    bool isRunningServer();
    bool isRunning();
    ofEvent<string>& clientMessageReceived();
    erTranslater* getTranslater();
    void onClientConnectionLost();
    void onClientMessageReceived(string& message);

protected:
    void send(erPlayParams& params, ofxNetworkSyncClientState* client);
    void send(string message, ofxNetworkSyncClientState* client);
    void sendChannelUpdates();
    void setLogLevels(ofLogLevel level);
    void drawBlackOverlay();

    ofxNetworkSyncServerFinder finder;
    ofxNetworkSyncClient client;
    ofxNetworkSyncServer server;
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
