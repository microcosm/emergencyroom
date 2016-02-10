#include "erNetwork.h"

void erNetwork::setup(){
    role = NETWORK_ROLE_UNDEFINED;
    drawingEnabled = false;
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);

    player.load("test-sound.mp3");
    player.setLoop(false);

    if(finder.setup()){
        finderStartTime = 0;
    }else{
        ofLogError("ofApp") << "failed to start finder";
        statusText = "failed to start finder";
    }

    statusText = "";
    serverPortOffset = 0;

    ofAddListener(ofEvents().update, this, &erNetwork::update);
    ofAddListener(ofEvents().draw, this, &erNetwork::draw);
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
                ofAddListener(client.messageReceived, this, &erNetwork::onMessageReceived);
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

void erNetwork::playTestSound(){
    if(server.hasClients()){
        bool bPlay = false;
        for(auto& client : server.getClients()) {
            if(client->isCalibrated()){
                bPlay = true;
                client->send("PLAY "+ofToString(server.getSyncedElapsedTimeMillis()+SOUND_PLAYER_DELAY));
            }
        }
        if(bPlay){
            player.play(SOUND_PLAYER_DELAY);
        }
    }
}

bool erNetwork::isRunningClient(){
    return role == NETWORK_ROLE_CLIENT;
}

bool erNetwork::isRunningServer(){
    return role == NETWORK_ROLE_SERVER;
}

//Client
void erNetwork::onMessageReceived(string& message){
    const string messagePlay = "PLAY ";
    if(message.find(messagePlay) == 0 && client.isCalibrated()){
        int time = ofToInt(message.substr(messagePlay.length()));
        player.play(time - client.getSyncedElapsedTimeMillis());
    }
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