#include "erMediaManager.h"

void erMediaManager::setup(){
    media.setup();
    ofAddListener(ofEvents().update, this, &erMediaManager::update);
    ofAddListener(ofEvents().draw, this, &erMediaManager::draw);
    seaLevelViz.setup();
}

void erMediaManager::play(erPlayParams params){
    if(params.isVideoCommand()){
        media.videoPlayers[params.getPath()]->execute(params);
    }else if(params.isTestCommand()){
        media.testSoundPlayer.execute(params);
        media.testVideoPlayer.execute(params);
    }
}

void erMediaManager::render(erPlayParams params){
    //Render graphics / dataviz
}

void erMediaManager::update(ofEventArgs& args){
    media.testVideoPlayer.update();
    for(auto const& player : media.videoPlayers){
        player.second->update();
    }
}

void erMediaManager::draw(ofEventArgs& args){
    ofClear(ofColor::black);
    if(media.testVideoPlayer.isPlaying()){
        media.testVideoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    for(auto const& player : media.videoPlayers){
        if(player.second->isPlaying()){
            player.second->draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
}

vector<string> erMediaManager::getVideoCollections(){
    return media.videoCollections;
}

map<string,vector<string>> erMediaManager::getCollectionsToVideos(){
    return media.collectionsToVideos;
}

vector<string> erMediaManager::getAllVideos(){
    return media.allVideos;
}