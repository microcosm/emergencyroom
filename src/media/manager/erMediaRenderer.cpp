#include "erMediaRenderer.h"

void erMediaRenderer::setup(erMedia* _media, erNetwork* _network, int _numChannels){
    media = _media;
    network = _network;
    numChannels = _numChannels;

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
        media->testVideoPlayer.draw(20, 20, ofGetWidth() - 40, ofGetHeight() - 40);
    }
    for(auto const& player : media->videoPlayers){
        if(player.second->isPlaying()){
            player.second->draw(20, 20, ofGetWidth() - 40, ofGetHeight() - 40);
        }
    }
}