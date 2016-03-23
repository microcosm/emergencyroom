#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network, int _numChannels){
    network = _network;
    numChannels = _numChannels;

    calculatePreviewSize();
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
    ofAddListener(ofEvents().draw, this, &erMediaRenderer::draw);
}

void erMediaRenderer::setTestSoundPlayer(erSyncedSoundPlayer* _testSoundPlayer){
    testSoundPlayer = _testSoundPlayer;
}

void erMediaRenderer::setTestVideoPlayer(erSyncedVideoPlayer* _testVideoPlayer){
    testVideoPlayer = _testVideoPlayer;
}

void erMediaRenderer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
}

void erMediaRenderer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
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
    previewBorderWidth = (ofGetWidth() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
    previewBorderHeight = (ofGetHeight() - SCREEN_MARGIN) / 3 - SCREEN_MARGIN;
}

void erMediaRenderer::drawClient(){
    ofClear(ofColor::black);
    drawTestVideoPlayer(0, 0, ofGetWidth(), ofGetHeight());
    for(auto const& player : *videoPlayers){
        if(player.second->isPlaying()){
            player.second->draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
}

void erMediaRenderer::drawServer(){
    ofClear(ofColor::black);
    drawTestVideoPlayer(SCREEN_MARGIN, SCREEN_MARGIN, ofGetWidth() - DOUBLE_SCREEN_MARGIN, ofGetHeight() - DOUBLE_SCREEN_MARGIN);
    
    ofSetColor(ofColor::white);
    ofNoFill();

    currentChannel = 1;
    for(int xi = 0; xi < 3; xi++){
        for(int yi = 0; yi < 3; yi++){
            drawPreviewBorder(xi, yi);
        }
    }
}

void erMediaRenderer::drawTestVideoPlayer(int x, int y, int width, int height){
    if(testVideoPlayer->isPlaying()){
        testVideoPlayer->draw(x, y, width, height);
    }
}

void erMediaRenderer::drawPreviewBorder(int xi, int yi){
    x = SCREEN_MARGIN * (xi + 1) + previewBorderWidth * xi;
    y = SCREEN_MARGIN * (yi + 1) + previewBorderHeight * yi;
    ofDrawRectangle(x, y, previewBorderWidth, previewBorderHeight);
    currentChannelStr = "CHANNEL " + ofToString(currentChannel++);
    ofDrawBitmapString(currentChannelStr, x + SCREEN_MARGIN, y + SCREEN_MARGIN + SCREEN_MARGIN);
}
