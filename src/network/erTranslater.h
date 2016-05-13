#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"
#include "erLogger.h"

class erTranslater{

public:
    void setup(ofxNetworkSyncClient* _client, ofxNetworkSyncServer* _server);
    erPlayParams toParams(string& messageStr);
    erPlayParams parseParams(erPlayParams params, vector<string>& messageParts);
    string toMessage(erPlayParams params);

protected:
    int getClientDelay(int serverDelay);
    ofxNetworkSyncClient* client;
    ofxNetworkSyncServer* server;
    vector<string> messageParts, argumentParts, variableParts;
};
