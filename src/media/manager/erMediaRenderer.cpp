#include "erMediaRenderer.h"

void erMediaRenderer::setup(erMedia* _media){
    media = _media;
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