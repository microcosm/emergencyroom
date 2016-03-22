#include "erMediaManager.h"

void erMediaManager::setup(int numChannels, erNetwork* network){
    media.setup();
    renderer.setup(&media, network, numChannels);
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

vector<string> erMediaManager::getVideoCollections(){
    return media.videoCollections;
}

map<string,vector<string>> erMediaManager::getCollectionsToVideos(){
    return media.collectionsToVideos;
}

vector<string> erMediaManager::getAllVideos(){
    return media.allVideos;
}