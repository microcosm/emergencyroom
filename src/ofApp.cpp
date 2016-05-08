#include "ofApp.h"

erSettings settings;

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(1200, 800);
    bigFont.load(ER_FONT_PATH, 100);
    smallFont.load(ER_FONT_PATH, 50);
    videoSoundAssigned = false;
    settings.load();
    if(settings.fullscreenByDefault){
        ofToggleFullscreen();
    }

    player.setTestSoundPlayer(&loader.testSoundPlayer);
    player.setTestVideoPlayer(&loader.testVideoPlayer);
    player.setVideoPaths(&loader.allVideos);
    player.setVideoPlayers(&loader.videoPlayers);
    player.setTexts(&loader.texts);

    if(settings.isEcg){
        player.setupEcgMode(&network);
        network.setup();
        sequencer.setupEcgMode(&network, &player);
    }else{
        loader.setup(&network);
        player.setup(&network);
        network.setup();
        sequencer.setup(&network, &loader, &player);
    }

    ofAddListener(ofEvents().draw, this, &ofApp::draw);
}

void ofApp::update(){
    width = ofGetWidth();
    height = ofGetHeight();

    if(loader.isLoaded() && !videoSoundAssigned){
        player.useSoundRendererFor(loader.audibleVideos);
        videoSoundAssigned = true;
    }

    if(network.isRunning() && settings.logToFileEnabled){
        erEnableFileLogging(network.isRunningServer());
    }else{
        erDisableFileLogging();
    }

    if(ofGetFrameNum() > 0){
        settings.isServer ? network.requestServer() : network.denyServer();
    }
}

void ofApp::draw(ofEventArgs& args){
    ofSetColor(ofColor::white);

    if(loader.hasErrors()){
        loader.drawErrors();
    }

    if(network.isRunningServer() && settings.serverDrawingEnabled){
        ofDrawBitmapString("v            toggle audio unit manager\n\nd            toggle server display\n\nD            toggle client display\n\nup/down      select ecg client\n\n-            sync to ecg client", 130, height - 168);
        drawFps(490, height - 60);
        smallFont.drawString(sequencer.getCurrentCollection(), 490, height - 220);
    }

    if((network.isRunningClient() || settings.isEcg) && settings.clientDrawingEnabled){
        ofDrawBitmapString("Computer name: '" + settings.computerName + "'\nDimensions:    " + ofToString(width) + " x " + ofToString(height), width - 250, 40);
        drawFps(320, height - 46);
    }
}

void ofApp::drawFps(int x, int y){
    bigFont.drawString("fps: " + ofToString(int(ofGetFrameRate())), x, y);
}

void ofApp::keyReleased(int key){
    if(network.isRunningServer()){
        if(key == 't'){
            params.newTestCommand();
            network.flood(params);
            player.playTest(params);
        }
        if(key == '1'){
            params.newVideoCommand();
            params.setPath("green/anti-fingers.mov");
            params.setSpeed(0.5);
            network.target(1, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '2'){
            params.newVideoCommand();
            params.setPath("purple/flip-fingers.mov");
            params.setSpeed(2);
            network.target(2, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
        if(key == '9'){
            params.newGraphicCommand();
            params.setPath("test");
            network.target(1, params);
            player.playTest(params);
            erLog("ofApp::keyReleased(int key)", params.getArgumentStr());
        }
    }else{
        if(key == 's'){
            network.requestServer();
        }
    }
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'd'){
        settings.toggleServerDrawing();
    }
    if(key == 'D'){
        settings.toggleClientDrawing();
        settings.clientDrawingEnabled ? network.clientDisplaysOn() : network.clientDisplaysOff();
    }
}