#include "erMediaRenderer.h"

void erMediaRenderer::setup(erMedia* _media, erNetwork* _network, int _numChannels){
    media = _media;
    network = _network;
    numChannels = _numChannels;

    calculatePreviewSize();
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
    ofAddListener(ofEvents().draw, this, &erMediaRenderer::draw);
}

void erMediaRenderer::update(ofEventArgs& args){
    media->testVideoPlayer.update();
    for(auto const& player : media->videoPlayers){
        player.second->update();
    }
}

void erMediaRenderer::draw(ofEventArgs& args){
    if(network->isRunningServer()){
        drawServer();
    }else{
        drawClient();
    }
}

void erMediaRenderer::calculatePreviewSize(){
    width = (ofGetWidth() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
    height = (ofGetHeight() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
}

void erMediaRenderer::drawClient(){
    ofClear(ofColor::black);
    if(media->testVideoPlayer.isPlaying()){
        media->testVideoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    for(auto const& player : media->videoPlayers){
        if(player.second->isPlaying()){
            player.second->draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
}

void erMediaRenderer::drawServer(){
    ofClear(ofColor::black);
    if(media->testVideoPlayer.isPlaying()){
        media->testVideoPlayer.draw(SCREEN_MARGIN, SCREEN_MARGIN, ofGetWidth() - SCREEN_MARGIN * 2, ofGetHeight() - SCREEN_MARGIN * 2);
    }

    ofSetColor(ofColor::white);
    ofNoFill();

    currentChannel = 1;
    for(int xi = 0; xi < 3; xi++){
        for(int yi = 0; yi < 3; yi++){
            x = SCREEN_MARGIN * (xi + 1) + width * xi;
            y = SCREEN_MARGIN * (yi + 1) + height * yi;

            ofDrawRectangle(x, y, width, height);

            currentChannelStr = "CHANNEL " + ofToString(currentChannel++);
            ofDrawBitmapString(currentChannelStr, x + SCREEN_MARGIN, y + SCREEN_MARGIN + SCREEN_MARGIN);
        }
    }
}