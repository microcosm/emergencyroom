#include "erMediaManager.h"

void erMediaManager::setup(){
    ensureSymlinkExists();
    loadTestMedia();
    loadProductionMedia();
    ofAddListener(ofEvents().update, this, &erMediaManager::update);
    ofAddListener(ofEvents().draw, this, &erMediaManager::draw);
    seaLevelViz.setup();
}

void erMediaManager::play(erPlayParams params){
    if(params.isVideoCommand()){
        videoPlayers[params.getPath()]->execute(params);
    }else if(params.isTestCommand()){
        testSoundPlayer.execute(params);
        testVideoPlayer.execute(params);
    }
}

void erMediaManager::render(erPlayParams params){
    
}

void erMediaManager::update(ofEventArgs& args){
    testVideoPlayer.update();
    for(auto const& player : videoPlayers){
        player.second->update();
    }
}

void erMediaManager::draw(ofEventArgs& args){
    ofClear(ofColor::black);
    if(testVideoPlayer.isPlaying()){
        testVideoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    for(auto const& player : videoPlayers){
        if(player.second->isPlaying()){
            player.second->draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }
}

vector<string> erMediaManager::getVideoCollections(){
    return videoCollections;
}

map<string,vector<string>> erMediaManager::getCollectionsToVideos(){
    return collectionsToVideos;
}

void erMediaManager::ensureSymlinkExists(){
    if(!ofFile::doesFileExist("dropbox")){
        ofSystem("ln -s ~/Dropbox/ ../../../data/dropbox");
    }
}

void erMediaManager::loadTestMedia(){
    testSoundPlayer.load(ER_TEST_SOUND);
    testSoundPlayer.setLoop(false);

    testVideoPlayer.load(ER_TEST_VIDEO);
    testVideoPlayer.setLoopState(OF_LOOP_NONE);
}

void erMediaManager::loadProductionMedia(){
    mediaDir = ofDirectory(ER_PRODUCTION_MEDIA_PATH);
    for(auto const& item : mediaDir){
        if(item.isDirectory()){
            loadDirectory(item.getAbsolutePath());
        }
    }
}

void erMediaManager::loadDirectory(string path){
    subDir = ofDirectory(path);
    subDir.allowExt(ER_ALLOWED_EXTENSIONS);
    if(subDir.listDir() > 0){
        folder = getBottomLevelFolder(subDir);
        registerVideoCollection(folder);
        for(auto const& file : subDir){
            registerVideo(folder, file);
        }
    }
}

void erMediaManager::registerVideo(string& folder, const ofFile file){
    path = getRelativePath(file);
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(file.getAbsolutePath());
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
    collectionsToVideos[folder].push_back(path);
}

void erMediaManager::registerVideoCollection(string& folder){
    videoCollections.push_back(folder);
    vector<string> videos;
    collectionsToVideos[folder] = videos;
}

string erMediaManager::getRelativePath(const ofFile file){
    vector<string> components = ofSplitString(file.getAbsolutePath(), "/");
    return components.at(components.size() - 2) + "/" + components.at(components.size() - 1);
}

string erMediaManager::getBottomLevelFolder(const ofDirectory directory){
    vector<string> components = ofSplitString(directory.getAbsolutePath(), "/");
    return components.at(components.size() - 1);
}