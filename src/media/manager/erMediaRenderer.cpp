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
    ofSetColor(ofColor::white);
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            ofDrawRectangle(
                SCREEN_MARGIN * (x + 1) + width * x,
                SCREEN_MARGIN * (y + 1) + height * y,
                width, height);
        }
    }
}