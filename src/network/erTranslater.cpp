#include "erTranslater.h"

/*  Sample message:
    VIDEO 5885460 path:to/video.mp4,speed:1
*/

void erTranslater::setup(ofxNetworkSyncClient* _client, ofxNetworkSyncServer* _server){
    client = _client;
    server = _server;
}

erPlayParams erTranslater::toParams(string& messageStr){
    erPlayParams params;
    messageParts = ofSplitString(messageStr, " ");

    if(client->isCalibrated() && messageParts.size() >= 2){
        params.newCommand(messageParts[0]);

        if(params.isPlayable()){
            params = parseParams(params, messageParts);
        }
    }

    return params;
}

erPlayParams erTranslater::parseParams(erPlayParams params, vector<string>& messageParts){
    params.setDelay(getClientDelay(ofToInt(messageParts[1])));
    if(messageParts.size() == 3){
        argumentParts = ofSplitString(messageParts[2], ",");
        variableParts = ofSplitString(argumentParts[0], ":");
        params.setPath(variableParts[1]);
        variableParts = ofSplitString(argumentParts[1], ":");
        params.setSpeed(ofToFloat(variableParts[1]));
    }
    erLog("erTranslater::parseParams(erPlayParams& params, vector<string>& messageParts)", "[Command: " + params.getCommandStr() + " | Delay: " + ofToString(params.getDelay()) + " | Speed: " + ofToString(params.getSpeed()) + " | Path: " + params.getPath() + "]");
    return params;
}

string erTranslater::toMessage(erPlayParams params){
    string message = params.getCommandStr() + " ";
    message += ofToString(server->getSyncedElapsedTimeMillis() + params.getDelay()) + " ";
    message += params.getArgumentStr();
    return message;
}

int erTranslater::getClientDelay(int serverDelay){
    return serverDelay - client->getSyncedElapsedTimeMillis();
}
