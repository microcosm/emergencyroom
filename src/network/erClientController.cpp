#include "erClientController.h"

void erClientController::setup(erNetwork* _network){
    if(settings.doManageClients){
        network = _network;
        frameCounter = 180;
    }
}

void erClientController::openClientApps(){
    if(settings.doManageClients){
        if(frameCounter <= 0){
            cleanRequests();
            vector<string> connectedClientIPs = network->getClientIPs();
            for(int i = 0; i < settings.clientIPs.size(); i++){
                if(!erContains(settings.clientIPs.at(i), connectedClientIPs)){
                    url = "http://" + settings.clientIPs.at(i) + ":" + ofToString(settings.managePort) + "/startOF";
                    request(url);
                }
            }
            frameCounter = ER_FRAMECOUNTER_MAX;
        }else{
            frameCounter--;
        }
    }
}

void erClientController::closeClientApps(){
    if(settings.doManageClients){
        cleanRequests();
        vector<string> connectedClientIPs = network->getClientIPs();
        for(int i = 0; i < connectedClientIPs.size(); i++){
            url = "http://" + connectedClientIPs.at(i) + ":" + ofToString(settings.managePort) + "/stopOF";
            ofLoadURL(url);
        }
    }
}

void erClientController::request(string url){
    erLog("erClientController::request(string url)", "Requesting: " + url);
    erHttpRequest* httpRequest = new erHttpRequest();
    httpRequests.push_back(httpRequest);
    httpRequests.back()->send(url);
}

void erClientController::cleanRequests(){
    for(int i = 0; i < httpRequests.size(); i++){
        httpRequests.at(i)->stopThread();
    }
    httpRequests.clear();
}
