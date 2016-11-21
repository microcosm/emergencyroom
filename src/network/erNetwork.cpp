#include "erNetwork.h"

void erNetwork::setup(){
    //string method = "erNetwork::setup()";
    signal(SIGPIPE, SIG_IGN);
    statusText = clientChannel = "";
    serverPortOffset = 0;
    numClients, previousNumClients = 0;
    ecgIndex = 0;
    font.load("font/klima-medium-web.ttf", 75);
    setLogLevels(OF_LOG_VERBOSE);
    translater.setup(&client, &server);

    if(settings.isServer) {
        //erLog(method, "Is server.");
        while(!server.setup(settings.serverPort)){
            //erLog(method, "Failed to set up server. Retrying...");
        }
    }

    ofAddListener(ofEvents().keyReleased, this, &erNetwork::keyReleased);
}

void erNetwork::update(){
    //string method = "erNetwork::update()";
    //erLog(method, "Called");

    if(settings.isServer) {
        server.update();
        numClients = server.getClients().size();
        if(numClients != previousNumClients){
            //erLog(method, "Sending channel updates.");
            sendChannelUpdates();
        }
        previousNumClients = numClients;
    } else {
        if(!client.isConnected()){
            if(client.setup(settings.serverIP, settings.serverPort)){
                ofAddListener(client.connectionLost, this, &erNetwork::onClientConnectionLost);
                ofAddListener(client.messageReceived, this, &erNetwork::onClientMessageReceived);
            }
        }
    }

    //erLog(method, "Done.");
}

void erNetwork::draw(){
    //string method = "erNetwork::draw()";
    //erLog(method, "Called");

    if(client.isConnected()){
        //erLog(method, "Client is connected...");
        if(settings.clientDrawingEnabled){
            //erLog(method, "Drawing enabled - Drawing now.");
            drawBlackOverlay();
            ofSetColor(ofColor::white);
            ofDrawBitmapString(statusText, 50, 30);
            client.drawStatus();
            ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
            ofDrawBitmapString(clientChannel, 50, ofGetHeight()-35);
            font.drawString(clientChannel, 40, ofGetHeight() * 0.4);
            //erLog(method, "Done drawing.");
        }else{
            //erLog(method, "Drawing not enabled. Done.");
        }
    }else if(server.isConnected()){
        //erLog(method, "Server is connected...");
        if(settings.serverDrawingEnabled){
            ofSetColor(ofColor::white);
            //erLog(method, "Drawing enabled - Drawing now.");
            ofDrawBitmapString(statusText, 50, 30);
            //erLog(method, "1");
            server.drawStatus(50, 50);
            //erLog(method, "2");
            vector<ofxNetworkSyncClientState *> clients = server.getClients();
            //erLog(method, "3");
            ostringstream ostr("");
            ostr << endl << endl;
            for(int i = 0; i < clients.size(); i++){
                if(i == ecgIndex){
                    ostr << "ECG>";
                }
                ostr << endl << endl;
            }
            //erLog(method, "4");
            ofDrawBitmapString(ostr.str(), 30, 50);
            //erLog(method, "5");
            ofDrawBitmapString(ofToString(server.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
            //erLog(method, "Done drawing.");
        }else{
            //erLog(method, "Drawing not enabled. Done.");
        }
    }

    //erLog(method, "Done.");
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
    //string method = "erNetwork::syncEcg(int delay)";
    //erLog(method, "Called");
    //erLog(method, "Ecg index = " + ofToString(ecgIndex) + ", Num clients = " + ofToString(server.getClients().size()));
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(settings.isServer && clients.size() > 0 && ecgIndex < clients.size()){
        string message = "ECGSYNC";
        //erLog(method, "Sending '" + message + "' to client " + ofToString(clients.at(ecgIndex)->getClientID()));
        send(message, clients.at(ecgIndex));
    }
    //erLog(method, "Done.");
}

int erNetwork::getClientId(){
    if(client.isConnected()){
        return client.getClientId();
    }
    return -1;
}

void erNetwork::clientStopAll(){
    //string method = "erNetwork::clientStopAll";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "STOP ALL";
            //erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            send(message, client);
        }
    }

    //erLog(method, "Done.");
}

void erNetwork::clientDisplaysOn(){
    //string method = "erNetwork::clientDisplaysOn";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY ON";
            //erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            send(message, client);
        }
    }

    //erLog(method, "Done.");
}

void erNetwork::clientDisplaysOff(){
    //string method = "erNetwork::clientDisplaysOff";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY OFF";
            //erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            send(message, client);
        }
    }

    //erLog(method, "Done.");
}

void erNetwork::requestServer(){
    serverRequested = true;
}

bool erNetwork::flood(erPlayParams params){
    //string method = "erNetwork::flood(erPlayParams params)";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    success = false;
    for(auto& client : server.getClients()) {
        send(params, client);
    }
    //erLog(method, "Done. Returning success? (" + ofToString(success) + ")");
    return success;
}

bool erNetwork::target(int target, erPlayParams params){
    //string method = "erNetwork::target(int target, erPlayParams params)";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));
    //erLog(method, "Target = " + ofToString(target) + ", Num clients = " + ofToString(server.getClients().size()));

    success = false;
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(target > 0 && target <= clients.size()){
        for(int i = target; i <= clients.size(); i += settings.numChannels) {
            ofxNetworkSyncClientState* client = clients[i-1];
            send(params, client);
        }
    }
    
    //erLog(method, "Done. Returning success? (" + ofToString(success) + ")");
    return success;
}

ofEvent<string>& erNetwork::clientMessageReceived(){
    return client.messageReceived;
}

erTranslater* erNetwork::getTranslater(){
    return &translater;
}

void erNetwork::onClientConnectionLost(){
    statusText = "lost connection to server";
    client.close();
    // retry to find server
    if(finder.setup()){
        finderStartTime = ofGetElapsedTimeMillis();
    }else{
        statusText += "\nfailed to start finder";
    }
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
    //string method = "erNetwork::send(erPlayParams& params, ofxNetworkSyncClientState* client)";
    //erLog(method, "Called");
    send(translater.toMessage(params), client);
    //erLog(method, "Done.");
}

void erNetwork::send(string message, ofxNetworkSyncClientState* client){
    //string method = "erNetwork::send(string& message, ofxNetworkSyncClientState* client)";
    //erLog(method, "Called");
    if(client->isCalibrated()){
        //erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
        try{
            client->send(message);
        }catch(...){
            //erLog(method, "Caught exception.");
        }
    }
    //erLog(method, "Done.");
}

void erNetwork::sendChannelUpdates(){
    //string method = "erNetwork::sendChannelUpdates";
    //erLog(method, "Called");
    //erLog(method, "Num clients = " + ofToString(server.getClients().size()));
    int i = 0;
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "CHANNEL " + ofToString((i % settings.numChannels) + 1);
            //erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            send(message, client);
            i++;
        }
    }
    //erLog(method, "Done.");
}

void erNetwork::setLogLevels(ofLogLevel level){
    ofSetLogLevel("erNetwork", level);
    ofSetLogLevel("ofxNetworkSyncClient", level);
    ofSetLogLevel("ofxNetworkSyncClientState", level);
    ofSetLogLevel("ofxNetworkSyncServer", level);
    ofSetLogLevel("ofxNetworkSyncServerFinder", level);
    ofSetLogLevel("ofxNetworkSyncUdp", level);
}

void erNetwork::drawBlackOverlay(){
    ofSetColor(ofColor::black, 127);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
}
