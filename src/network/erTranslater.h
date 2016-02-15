#pragma once

#include "ofMain.h"
#include "ofxNetworkSync.h"
#include "erPlayParams.h"

class erTranslater{

public:
    void setup(ofxNetworkSyncClient* _client, ofxNetworkSyncServer* _server);
    erPlayParams toParams(string& messageStr);
    string toMessage(erPlayParams& params);

protected:
    int getClientDelay(int serverDelay);
    ofxNetworkSyncClient* client;
    ofxNetworkSyncServer* server;
    vector<string> messageParts, argumentParts, variableParts;
};
