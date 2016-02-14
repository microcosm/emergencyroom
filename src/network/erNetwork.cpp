#include "erNetwork.h"

void erNetwork::setup(int _defaultDelay){
    defaultDelay = _defaultDelay;
    role = NETWORK_ROLE_UNDEFINED;
    numChannels = 1;
    drawingEnabled = false;
    statusText = "";
    serverPortOffset = 0;
    setLogLevels(OF_LOG_ERROR);

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

        if(now > finderStartTime+FINDER_TIMEOUT){
            //server finder timeout

            //i will be server
            if(server.setup(SYNC_TCP_PORT+serverPortOffset)){
                role = NETWORK_ROLE_SERVER;
                statusText = "i am server";
                //ofRemoveListener(finder.serverFound, this, &ofApp::onServerFound);
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

void erNetwork::enableDrawing(){
    drawingEnabled = true;
}

bool erNetwork::flood(string command){
    return flood(command, defaultDelay);
}

bool erNetwork::flood(string command, int delay){
    success = false;
    for(auto& client : server.getClients()) {
        send(format(command, delay), client);
    }
    return success;
}

bool erNetwork::target(int target, string command, string arguments){
    return this->target(target, command, arguments, defaultDelay);
}

bool erNetwork::target(int target, string command, string arguments, int delay){
    success = false;
    vector<ofxNetworkSyncClientState*>& clients = server.getClients();
    if(target > 0 && target <= clients.size()){
        for(int i = target; i <= clients.size(); i+=numChannels) {
            ofxNetworkSyncClientState* client = clients[i-1];
            send(format(command, delay, arguments), client);
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

bool erNetwork::isClientReady(){
    return client.isCalibrated();
}

int erNetwork::getClientDelay(int serverDelay){
    return serverDelay - client.getSyncedElapsedTimeMillis();
}

ofxNetworkSyncClient* erNetwork::getClient(){
    return &client;
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

void erNetwork::send(string message, ofxNetworkSyncClientState* client){
    if(client->isCalibrated()){
        success = true;
        client->send(message);
    }
}

string erNetwork::format(string command, int delay, string arguments){
    string formatted = command + " ";
    formatted += ofToString(server.getSyncedElapsedTimeMillis() + delay);
    formatted += arguments.length() > 0 ? " " + arguments : "";
    return formatted;
}

void erNetwork::setLogLevels(ofLogLevel level){
    ofSetLogLevel("erNetwork", level);
    ofSetLogLevel("ofxNetworkSyncClient", level);
    ofSetLogLevel("ofxNetworkSyncClientState", level);
    ofSetLogLevel("ofxNetworkSyncServer", level);
    ofSetLogLevel("ofxNetworkSyncServerFinder", level);
    ofSetLogLevel("ofxNetworkSyncUdp", level);
}