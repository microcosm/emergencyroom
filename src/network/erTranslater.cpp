#include "erTranslater.h"

void erTranslater::setup(ofxNetworkSyncClient* _client, ofxNetworkSyncServer* _server){
    client = _client;
    server = _server;
}

erPlayParams erTranslater::toParams(string& messageStr){
    erPlayParams params;
    messageParts = ofSplitString(messageStr, " ");

    if(client->isCalibrated() && messageParts.size() >= 2){
        params.newCommand(messageParts[0]);
        params.setDelay(getClientDelay(ofToInt(messageParts[1])));
        if(messageParts.size() == 3){
            argumentParts = ofSplitString(messageParts[2], ",");
            variableParts = ofSplitString(argumentParts[0], "=");
            params.setSpeed(ofToFloat(variableParts[1]));
        }
    }

    return params;
}

string erTranslater::toMessage(erPlayParams& params){
    string message = params.getCommandStr() + " ";
    message += ofToString(server->getSyncedElapsedTimeMillis() + params.getDelay()) + " ";
    message += params.getArgumentStr();
    return message;
}

int erTranslater::getClientDelay(int serverDelay){
    return serverDelay - client->getSyncedElapsedTimeMillis();
}