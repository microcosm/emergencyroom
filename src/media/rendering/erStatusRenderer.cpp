#include "erStatusRenderer.h"

void erStatusRenderer::setup(erNetwork* _network, erMediaSequencer* _mediaSequencer, erMediaController* _mediaController){
    network = _network;
    mediaSequencer = _mediaSequencer;
    mediaController = _mediaController;

    width = ofGetWidth();
    height = ofGetHeight();

    bigFont.load(settings.fontPath, 75);
    smallFont.load(settings.fontPath, 50);
}

void erStatusRenderer::draw(){
    if(settings.isServer && settings.renderChannels()){
        //Status is rendered over channels on the server
        drawOverlay();
        network->draw();
        mediaSequencer->draw();

        ofDrawBitmapString("v            toggle audio unit manager\n\nn            increment server view mode\n\nN            toggle client view mode\n\n", 130, height - 168);
        drawFps(490, height - 60);
        smallFont.drawString(mediaSequencer->getCurrentCollection(), 490, height - 220);
    }

    if(settings.isClient && settings.renderStatus()){
        drawOverlay();
        network->draw();

        ofDrawBitmapString("Computer name: '" + settings.computerName + "'\nDimensions:    " + ofToString(width) + " x " + ofToString(height), width - 350, 40);
        drawFps(320, height - 46);

        int clientId = network->getClientId();
        if(clientId != -1){
            ofDrawBitmapString("Client ID:", 50, height - 148);
            smallFont.drawString("#" + ofToString(network->getClientId()), 50, height - 90);
            ofDrawBitmapString("Playback state:", 50, height - 236);
            ofDrawBitmapString(mediaController->getClientVideoState(), 50, height - 220);
        }
    }
}

void erStatusRenderer::drawFps(int x, int y){
    bigFont.drawString("fps: " + ofToString(int(ofGetFrameRate())), x, y);
}

void erStatusRenderer::drawOverlay(){
    ofSetColor(ofColor::white);
    ofFill();
    ofSetColor(ofColor::black, 100);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
}
