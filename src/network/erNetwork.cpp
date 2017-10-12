#include "erNetwork.h"

void erNetwork::setup(){
    signal(SIGPIPE, SIG_IGN);
    statusText = clientChannel = "";
    serverPortOffset = 0;
    numClients, previousNumClients = 0;
    font.load("font/klima-medium-web.ttf", 75);
    setLogLevels(OF_LOG_VERBOSE);
    translater.setup(&client, &server);

    if(settings.isServer) {
        while(!server.setup(settings.ofPort)){
            erLog("erNetwork::setup()", "Failed to set up server. Retrying...");
        }
    }
}

void erNetwork::update(){
    if(settings.isServer) {
        server.update();
        numClients = server.getClients().size();
        if(numClients != previousNumClients){
            sendChannelUpdates();
        }
        previousNumClients = numClients;
    } else {
        if(!client.isConnected()){
            if(client.setup(settings.serverIP, settings.ofPort)){
                ofAddListener(client.messageReceived, this, &erNetwork::onClientMessageReceived);
            }
        }
    }
}

void erNetwork::draw(){
    if(settings.isClient && settings.clientDrawingEnabled){
        drawOverlay();
        drawClient();
    }
    if(settings.isServer){
        drawOverlay();
        drawServer();
    }
}

void erNetwork::drawClient(){
    if(client.isConnected()){
        ofDrawBitmapString(statusText, 50, 30);
        client.drawStatus();
        ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
        ofDrawBitmapString(clientChannel, 50, ofGetHeight()-35);
        font.drawString(clientChannel, 40, ofGetHeight() * 0.4);
    }else{
        ofDrawBitmapString("Waiting for server...", 50, 30);
    }
}

void erNetwork::drawServer(){
    if(server.isConnected()){
        ofSetColor(ofColor::white);
        ofDrawBitmapString(statusText, 50, 30);
        server.drawStatus(50, 50);
        ofDrawBitmapString(ofToString(server.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
    }else{
        ofDrawBitmapString("Setting up server network connection...", 50, 30);
    }
}

void erNetwork::drawOverlay(){
    ofSetColor(ofColor::black, 127);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor::white);
}

int erNetwork::getClientId(){
    if(client.isConnected()){
        return client.getClientId();
    }
    return -1;
}

vector<string> erNetwork::getClientIPs(){
    vector<ofxNetworkSyncClientState *> clients = server.getClients();
    vector<string> clientIPs;
    for(int i = 0; i < clients.size(); i++){
        clientIPs.push_back(clients.at(i)->getIpAddr());
    }
    return clientIPs;
}

void erNetwork::clientStopAll(){
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "STOP ALL";
            send(message, client);
        }
    }
}

void erNetwork::clientDisplaysOn(){
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY ON";
            send(message, client);
        }
    }
}

void erNetwork::clientDisplaysOff(){
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY OFF";
            send(message, client);
        }
    }
}

void erNetwork::requestServer(){
    serverRequested = true;
}

bool erNetwork::flood(erPlayParams params){
    success = false;
    for(auto& client : server.getClients()) {
        send(params, client);
    }
    return success;
}

bool erNetwork::target(int target, erPlayParams params){
    success = false;
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(target > 0 && target <= clients.size()){
        for(int i = target; i <= clients.size(); i += settings.numChannels) {
            ofxNetworkSyncClientState* client = clients[i-1];
            send(params, client);
        }
    }
    return success;
}

ofEvent<string>& erNetwork::clientMessageReceived(){
    return client.messageReceived;
}

erTranslater* erNetwork::getTranslater(){
    return &translater;
}

void erNetwork::onClientMessageReceived(string& message){
    if(message.substr(0, 7) == "CHANNEL"){
        clientChannel = message;
    }

    if(message.substr(0, 10) == "DISPLAY ON"){
        settings.clientDrawingEnabled = true;
        ofShowCursor();
    }

    if(message.substr(0, 11) == "DISPLAY OFF"){
        settings.clientDrawingEnabled = false;
        ofHideCursor();
    }
}

void erNetwork::send(erPlayParams& params, ofxNetworkSyncClientState* client){
    send(translater.toMessage(params), client);
}

void erNetwork::send(string message, ofxNetworkSyncClientState* client){
    if(client->isCalibrated()){
        try{
            client->send(message);
        }catch(...){
            erLog("erNetwork::send(string message, ofxNetworkSyncClientState* client)", "Caught exception.");
        }
    }
}

void erNetwork::sendChannelUpdates(){
    int i = 0;
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "CHANNEL " + ofToString((i % settings.numChannels) + 1);
            send(message, client);
            i++;
        }
    }
}

void erNetwork::setLogLevels(ofLogLevel level){
    ofSetLogLevel("erNetwork", level);
    ofSetLogLevel("ofxNetworkSyncClient", level);
    ofSetLogLevel("ofxNetworkSyncClientState", level);
    ofSetLogLevel("ofxNetworkSyncServer", level);
    ofSetLogLevel("ofxNetworkSyncServerFinder", level);
    ofSetLogLevel("ofxNetworkSyncUdp", level);
}
