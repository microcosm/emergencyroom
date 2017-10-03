#include "erClientController.h"

void erClientController::setup(erNetwork* _network){
    network = _network;
    frameCounter = 0;
}

void erClientController::launchClients(){
    if(frameCounter <= 0){
        vector<string> connectedClientIPs = network->getClientIPs();
        for(int i = 0; i < settings.clientIPs.size(); i++){
            if(!erContains(settings.clientIPs.at(i), connectedClientIPs)){
                ofLoadURL("http://" + settings.clientIPs.at(i) + ":" + ofToString(settings.managePort) + "/startOF");
            }
        }
        frameCounter = ER_FRAMECOUNTER_MAX;
    }else{
        frameCounter--;
    }
}
