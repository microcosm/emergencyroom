#include "erNetwork.h"

void erNetwork::setup(){
    role = NETWORK_ROLE_UNDEFINED;
    statusText = clientChannel = "";
    serverPortOffset = 0;
    serverRequested = false;
    serverIsAllowed = true;
    numClients, previousNumClients = 0;
    ecgIndex = 0;
    font.load("font/klima-medium-web.ttf", 100);
    setLogLevels(OF_LOG_ERROR);
    translater.setup(&client, &server);

    if(finder.setup()){
        finderStartTime = 0;
    }else{
        ofLogError("ofApp") << "failed to start finder";
        statusText = "failed to start finder";
    }

    ofAddListener(ofEvents().update, this, &erNetwork::update);
    ofAddListener(ofEvents().draw, this, &erNetwork::draw);
    ofAddListener(ofEvents().keyReleased, this, &erNetwork::keyReleased);
}

void erNetwork::update(ofEventArgs& args){
    string method = "erNetwork::update(ofEventArgs& args)";
    erLog(method, "Called");

    previousRole = role;
    now = ofGetElapsedTimeMillis();

    if(!server.isConnected() && !client.isConnected() && !finder.isRunning()){
        erLog(method, "Nothing running or connected. Trying to start finder.");

        if(!finder.setup()){
            erLog(method, "Finder couldn't start.");
            statusText += "\nfailed to start finder";
        }
    }

    if(finder.isRunning()){
        erLog(method, "Finder is running.");
        if(finder.doesServerFound()){
            
            erLog(method, "Server was found.");
            
            statusText = "server found";
            finder.close();
            if(client.setup(finder.getServerInfo().ip, finder.getServerInfo().port)){
                erLog(method, "Client setup succeeded.");
                role = NETWORK_ROLE_CLIENT;
                ofAddListener(client.connectionLost, this, &erNetwork::onClientConnectionLost);
                ofAddListener(client.messageReceived, this, &erNetwork::onClientMessageReceived);
            }else{
                
                erLog(method, "Client setup failed.");
            }
        }else{
            
            erLog(method, "No server found.");
        }

        if(serverIsAllowed){
            erLog(method, "Server is allowed.");
            if(now > finderStartTime+FINDER_TIMEOUT || serverRequested){
                //server finder timeout
                serverRequested = false;

                //i will be server
                if(server.setup(SYNC_TCP_PORT+serverPortOffset)){
                    
                    erLog(method, "Server setup OK.");
                    role = NETWORK_ROLE_SERVER;
                    statusText = "i am server";
                    finder.close();

                    if(client.isCalibrated()){
                        erLog(method, "Client is calibrated. (?)");
                        server.setTimeOffsetMillis(client.getSyncedElapsedTimeMillis()-ofGetElapsedTimeMillis());
                    }
                }else{
                    //failed to start server. still try to find server
                    
                    erLog(method, "Server failed to start.");
                    statusText = "server failed to start. maybe given address is already in use?";
                    erLog(method, "1");
                    finderStartTime = now;
                    erLog(method, "2");
                    server.close();
                    erLog(method, "3");
                    serverPortOffset++;
                    erLog(method, "4.");
                }
            }
        }else{
            erLog(method, "Server not allowed. Restarting timer.");
            finderStartTime = now;
        }
    }else if(server.isConnected()){
        erLog(method, "Server is connected. Updating.");
        server.update();
        erLog(method, "Updated.");
        numClients = server.getClients().size();
        erLog(method, "Num clients = " + ofToString(numClients));
        if(numClients != previousNumClients){
            erLog(method, "Sending channel updates.");
            sendChannelUpdates();
        }
        previousNumClients = numClients;
    }
    
    erLog(method, "Done.");
}

void erNetwork::draw(ofEventArgs& args){
    string method = "erNetwork::draw(ofEventArgs& args)";
    erLog(method, "Called");

    if(finder.isRunning()){
        erLog(method, "Finder is running. Drawing.");
        drawBlackOverlay();
        ofSetColor(ofColor::white);
        ofDrawBitmapString(statusText, 50, 30);
        ofDrawBitmapString("trying to find server: " + ofToString((finderStartTime+FINDER_TIMEOUT)-ofGetElapsedTimeMillis()), 50, 50);
        ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
        erLog(method, "Done drawing.");
    }else if(client.isConnected()){
        erLog(method, "Client is connected...");
        if(settings.clientDrawingEnabled){
            erLog(method, "Drawing enabled - Drawing now.");
            drawBlackOverlay();
            ofSetColor(ofColor::white);
            ofDrawBitmapString(statusText, 50, 30);
            client.drawStatus();
            ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
            ofDrawBitmapString(clientChannel, 50, ofGetHeight()-35);
            font.drawString(clientChannel, 40, ofGetHeight() * 0.4);
            erLog(method, "Done drawing.");
        }else{
            erLog(method, "Drawing not enabled. Done.");
        }
    }else if(server.isConnected()){
        erLog(method, "Server is connected...");
        if(settings.serverDrawingEnabled){
            erLog(method, "Drawing enabled - Drawing now.");
            ofDrawBitmapString(statusText, 50, 30);
            erLog(method, "1");
            server.drawStatus(50, 50);
            erLog(method, "2");
            vector<ofxNetworkSyncClientState *> clients = server.getClients();
            erLog(method, "3");
            ostringstream ostr("");
            ostr << endl << endl;
            for(int i = 0; i < clients.size(); i++){
                if(i == ecgIndex){
                    ostr << "ECG>";
                }
                ostr << endl << endl;
            }
            erLog(method, "4");
            ofDrawBitmapString(ostr.str(), 30, 50);
            erLog(method, "5");
            ofDrawBitmapString(ofToString(server.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
            erLog(method, "Done drawing.");
        }else{
            erLog(method, "Drawing not enabled. Done.");
        }
    }

    erLog(method, "Done.");
}

void erNetwork::keyReleased(ofKeyEventArgs &args){
    if(isRunningServer()){
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
    string method = "erNetwork::syncEcg(int delay)";
    erLog(method, "Called");
    erLog(method, "Ecg index = " + ofToString(ecgIndex) + ", Num clients = " + ofToString(server.getClients().size()));
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(isRunningServer() && clients.size() > 0 && ecgIndex < clients.size()){
        string message = "ECGSYNC";
        erLog(method, "Sending '" + message + "' to client " + ofToString(clients.at(ecgIndex)->getClientID()));
        clients.at(ecgIndex)->send(message);
    }
    erLog(method, "Done.");
}

int erNetwork::getClientId(){
    if(client.isConnected()){
        return client.getClientId();
    }
    return -1;
}

void erNetwork::clientStopAll(){
    string method = "erNetwork::clientStopAll";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "STOP ALL";
            erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            client->send(message);
        }
    }

    erLog(method, "Done.");
}

void erNetwork::clientDisplaysOn(){
    string method = "erNetwork::clientDisplaysOn";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY ON";
            erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            client->send(message);
        }
    }

    erLog(method, "Done.");
}

void erNetwork::clientDisplaysOff(){
    string method = "erNetwork::clientDisplaysOff";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "DISPLAY OFF";
            erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            client->send(message);
        }
    }

    erLog(method, "Done.");
}

void erNetwork::requestServer(){
    serverRequested = true;
}

bool erNetwork::flood(erPlayParams params){
    string method = "erNetwork::flood(erPlayParams params)";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));

    success = false;
    for(auto& client : server.getClients()) {
        send(params, client);
    }
    erLog(method, "Done. Returning success? (" + ofToString(success) + ")");
    return success;
}

bool erNetwork::target(int target, erPlayParams params){
    string method = "erNetwork::target(int target, erPlayParams params)";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));
    erLog(method, "Target = " + ofToString(target) + ", Num clients = " + ofToString(server.getClients().size()));

    success = false;
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(target > 0 && target <= clients.size()){
        for(int i = target; i <= clients.size(); i += settings.numChannels) {
            ofxNetworkSyncClientState* client = clients[i-1];
            send(params, client);
        }
    }
    
    erLog(method, "Done. Returning success? (" + ofToString(success) + ")");
    return success;
}

bool erNetwork::wasRunningClient(){
    return previousRole == NETWORK_ROLE_CLIENT;
}

bool erNetwork::wasRunningServer(){
    return previousRole == NETWORK_ROLE_SERVER;
}

bool erNetwork::isRunningClient(){
    return role == NETWORK_ROLE_CLIENT;
}

bool erNetwork::isRunningServer(){
    return role == NETWORK_ROLE_SERVER;
}

bool erNetwork::isRunning(){
    return isRunningClient() || isRunningServer();
}

bool erNetwork::justBecameClient(){
    return isRunningClient() && !wasRunningClient();
}

void erNetwork::denyServer(){
    serverIsAllowed = false;
}

bool erNetwork::justBecameServer(){
    return isRunningServer() && !wasRunningServer();
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
    string method = "erNetwork::send(erPlayParams& params, ofxNetworkSyncClientState* client)";
    erLog(method, "Called");
    if(client->isCalibrated()){
        success = true;
        string message = translater.toMessage(params);
        erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
        client->send(message);
    }
    erLog(method, "Done.");
}

void erNetwork::sendChannelUpdates(){
    string method = "erNetwork::sendChannelUpdates";
    erLog(method, "Called");
    erLog(method, "Num clients = " + ofToString(server.getClients().size()));
    int i = 0;
    for(auto& client : server.getClients()) {
        if(client->isCalibrated()){
            string message = "CHANNEL " + ofToString((i % settings.numChannels) + 1);
            erLog(method, "Sending '" + message + "' to client " + ofToString(client->getClientID()));
            client->send(message);
            i++;
        }
    }
    erLog(method, "Done.");
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