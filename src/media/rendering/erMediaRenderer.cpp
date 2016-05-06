#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network){
    erGlitchRenderer::setup();
    network = _network;
    bufferEmpty = true;
    decoyFramesRemaining = 0;
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fboGlitch.allocate(ofGetWidth(), ofGetHeight());
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
}

void erMediaRenderer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
        player.second->update();
    }
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

void erMediaRenderer::assignDecoyGlitch(erSyncedVideoPlayer* _videoPlayer){
    decoyGlitchPlayer = _videoPlayer;
    decoyGlitchPlayer->play();
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height, int channel){
    if(player->isPlaying()){
        withinGlitchPeriod(channel) && network->isRunningClient() ?
            drawGlitched(player, x, y, width, height) :
            drawNormal(player, x, y, width, height);
    }
}

void erMediaRenderer::drawStatic(int x, int y, int width, int height){
    if(bufferEmpty || ofRandom(1) < 0.05){
        fbo.begin();
        {
            ofClear(ofColor::black);
            for(int i = 0; i < 50; i++){
                ofSetColor(ofColor::white, ofRandom(160));
                x = ofRandomWidth();
                y = ofRandomHeight();
                ofDrawRectangle(x, y, ofRandom(ofGetWidth() - x), ofRandom(ofGetHeight() - y));
            }
        }
        fbo.end();
        bufferEmpty = false;
    }
    fboGlitch.draw(fbo, x, y, width, height);
}

void erMediaRenderer::drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    player->draw(x, y, width, height);
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    fbo.begin();
    {
        if(decoyFramesRemaining > 0 && decoyGlitchPlayer->isPlaying()){
            decoyGlitchPlayer->update();
            decoyGlitchPlayer->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            decoyFramesRemaining--;
        }else{
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
        }

        if(decoyFramesRemaining == 0 && ofRandom(1) < 0.05){
            decoyFramesRemaining = floor(ofRandom(2, 5));
        }
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, width, height);
}