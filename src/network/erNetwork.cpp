#include "erNetwork.h"

void erNetwork::setup(){
    role = NETWORK_ROLE_UNDEFINED;
    numChannels = 1;
    drawingEnabled = false;
    statusText = "";
    serverPortOffset = 0;
    serverRequested = false;
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
}

void erNetwork::setNumChannels(int _numChannels){
    numChannels = _numChannels;
}

void erNetwork::update(ofEventArgs& updateArgs){
    now = ofGetElapsedTimeMillis();

    if(!server.isConnected() && !client.isConnected() && !finder.isRunning()){
        if(!finder.setup()){
            statusText += "\nfailed to start finder";
        }
    }

    if(finder.isRunning()){
        if(finder.doesServerFound()){
            statusText = "server found";
            finder.close();
            if(client.setup(finder.getServerInfo().ip, finder.getServerInfo().port)){
                role = NETWORK_ROLE_CLIENT;
                ofAddListener(client.connectionLost, this, &erNetwork::onConnectionLost);
            }
        }

        if(now > finderStartTime+FINDER_TIMEOUT || serverRequested){
            //server finder timeout
            serverRequested = false;

            //i will be server
            if(server.setup(SYNC_TCP_PORT+serverPortOffset)){
                role = NETWORK_ROLE_SERVER;
                statusText = "i am server";
                finder.close();
                
                if(client.isCalibrated()){
                    server.setTimeOffsetMillis(client.getSyncedElapsedTimeMillis()-ofGetElapsedTimeMillis());
                }
            }else{
                //failed to start server. still try to find server
                statusText = "server failed to start. maybe given address is already in use?";
                finderStartTime = now;
                server.close();
                serverPortOffset++;
            }
        }
    }else if(server.isConnected()){
        server.update();
    }
}

void erNetwork::draw(ofEventArgs& updateArgs){
    if(drawingEnabled){
        ofFill();
        ofSetColor(ofColor::black, 100);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(ofColor::white);
        ofDrawBitmapString(statusText, 50, 30);
        if(finder.isRunning()){
            ofDrawBitmapString("trying to find server: " + ofToString((finderStartTime+FINDER_TIMEOUT)-ofGetElapsedTimeMillis()), 50, 50);
            ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
        }else if(client.isConnected()){
            client.drawStatus();
            ofDrawBitmapString(ofToString(client.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
        }else if(server.isConnected()){
            server.drawStatus();
            ofDrawBitmapString(ofToString(server.getSyncedElapsedTimeMillis()), 50, ofGetHeight()-70);
        }
    }
}

void erNetwork::toggleDrawing(){
    drawingEnabled = !drawingEnabled;
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
        for(int i = target; i <= clients.size(); i+=numChannels) {
            ofxNetworkSyncClientState* client = clients[i-1];
            send(params, client);
        }
    }
    return success;
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

ofEvent<string>& erNetwork::clientMessageReceived(){
    return client.messageReceived;
}

erTranslater* erNetwork::getTranslater(){
    return &translater;
}

int erNetwork::getNumChannels(){
    return numChannels;
}

void erNetwork::onConnectionLost(){
    statusText = "lost connection to server";
    client.close();
    // retry to find server
    if(finder.setup()){
        finderStartTime = ofGetElapsedTimeMillis();
    }else{
        statusText += "\nfailed to start finder";
    }
}

void erNetwork::send(erPlayParams& params, ofxNetworkSyncClientState* client){
    if(client->isCalibrated()){
        success = true;
        client->send(translater.toMessage(params));
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