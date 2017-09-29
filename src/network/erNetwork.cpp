#include "erNetwork.h"

void erNetwork::setup(){
    signal(SIGPIPE, SIG_IGN);
    statusText = clientChannel = "";
    serverPortOffset = 0;
    numClients, previousNumClients = 0;
    ecgIndex = 0;
    font.load("font/klima-medium-web.ttf", 75);
    setLogLevels(OF_LOG_VERBOSE);
    translater.setup(&client, &server);

    if(settings.isServer) {
        while(!server.setup(settings.serverPort)){
            erLog("erNetwork::setup()", "Failed to set up server. Retrying...");
        }
    }

    ofAddListener(ofEvents().keyReleased, this, &erNetwork::keyReleased);
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
            if(client.setup(settings.serverIP, settings.serverPort)){
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
    if(settings.isServer && settings.serverDrawingEnabled){
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
        drawClientList();
        ofDrawBitmapString(ofToString(server.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
    }else{
        ofDrawBitmapString("Setting up server network connection...", 50, 30);
    }
}

void erNetwork::drawClientList(){
    vector<ofxNetworkSyncClientState *> clients = server.getClients();
    ostringstream ostr("");
    ostr << endl << endl;
    for(int i = 0; i < clients.size(); i++){
        if(i == ecgIndex){
            ostr << "ECG>";
        }
        ostr << endl << endl;
    }
    ofDrawBitmapString(ostr.str(), 30, 50);
}

void erNetwork::drawOverlay(){
    ofSetColor(ofColor::black, 127);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor::white);
}

void erNetwork::keyReleased(ofKeyEventArgs &args){
    if(settings.isServer){
        if(args.key == 359){
            ecgIndex++;
            if(ecgIndex >= server.getClients().size()){
                ecgIndex = 0;
            }
        }

        if(args.key == 357){
            ecgIndex--;
            if(ecgIndex < 0){
                ecgIndex = server.getClients().size() - 1;
            }
        }
    }
}

void erNetwork::syncEcg(int delay){
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(settings.isServer && clients.size() > 0 && ecgIndex < clients.size()){
        string message = "ECGSYNC";
        send(message, clients.at(ecgIndex));
    }
}

int erNetwork::getClientId(){
    if(client.isConnected()){
        return client.getClientId();
    }
    return -1;
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
