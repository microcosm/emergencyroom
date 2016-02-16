#include "erMediaManager.h"

void erMediaManager::setup(){
    loadTestMedia();
    loadProductionMedia();
    ofAddListener(ofEvents().update, this, &erMediaManager::update);
    ofAddListener(ofEvents().draw, this, &erMediaManager::draw);
}

void erMediaManager::play(erPlayParams params){
    if(params.isPlayCommand()) {
        cout << "play something" << endl;
    }else if(params.isTestCommand()) {
        testSoundPlayer.execute(params);
        testVideoPlayer.execute(params);
    }
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
    for(auto const& file : subDir){
        pushVideo(file);
    }
}

void erMediaManager::pushVideo(const ofFile file){
    path = file.getAbsolutePath();
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(path);
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
}