#include "erMediaManager.h"

void erMediaManager::setup(){
    loadTestVideos();
    ofAddListener(ofEvents().update, this, &erMediaManager::update);
    ofAddListener(ofEvents().draw, this, &erMediaManager::draw);
}

void erMediaManager::play(erPlayParams params){
    if(params.isTestCommand()) {
        soundPlayers[0]->execute(params);
        videoPlayers[0]->execute(params);
    }
}

void erMediaManager::update(ofEventArgs& args){
    videoPlayers[0]->update();
}

void erMediaManager::draw(ofEventArgs& args){
    ofClear(ofColor::black);
    if(videoPlayers[0]->isPlaying()){
        videoPlayers[0]->draw(0, 0, ofGetWidth(), ofGetHeight());
    }
}

void erMediaManager::loadTestVideos(){
    soundPlayers.push_back(ofPtr<erSyncedSoundPlayer>(new erSyncedSoundPlayer));
    soundPlayers[0]->load(ER_TEST_SOUND);
    soundPlayers[0]->setLoop(false);

    videoPlayers.push_back(ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer));
    videoPlayers[0]->load(ER_TEST_VIDEO);
    videoPlayers[0]->setLoopState(OF_LOOP_NONE);
}