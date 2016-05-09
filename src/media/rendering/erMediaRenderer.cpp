#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network){
    erGlitchTimer::setup();
    network = _network;
    bufferEmpty = true;
    decoyFramesRemaining = 0;
    minDecoyFrames = 3;
    maxDecoyFrames = 7;
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fboGlitch.allocate(ofGetWidth(), ofGetHeight());
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
}

void erMediaRenderer::update(ofEventArgs& args){
    testVideoPlayer->update();
    for(auto const& player : *videoPlayers){
        player.second->update();
    }
    if(network->isRunningClient() && decoyGlitchPlayer != NULL){
        updateDecoyPlayer();
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
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height, int channel){
    if(player->isPlaying()){
        withinGlitchPeriod(channel) && network->isRunningClient() ?
            drawGlitched(player, x, y, width, height) :
            drawNormal(player, x, y, width, height);
    }
}

void erMediaRenderer::drawStatic(int x, int y, int width, int height){
    if(bufferEmpty || ofRandom(1) < 0.02){
        fbo.begin();
        {
            ofClear(ofColor::black);
            for(int i = 0; i < 20; i++){
                ofSetColor(ofRandom(230, 255), ofRandom(230, 255), ofRandom(230, 255), ofRandom(1, 5) * 50);
                x = ofRandom(ofGetWidth() * 2) - ofGetWidth() * 0.5;
                y = ofRandom(ofGetHeight() * 2) - ofGetHeight() * 0.5;
                ofDrawRectangle(x, y, ofRandom(ofGetWidth() - x), ofRandom(ofGetHeight() - y));
            }
        }
        fbo.end();
        bufferEmpty = false;
    }
    fboGlitch.draw(fbo, x, y, width, height);
    playbackState = ER_PLAYBACK_STATIC;
}

void erMediaRenderer::drawNormal(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    player->draw(x, y, width, height);
    playbackState = ER_PLAYBACK_NORMAL;
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    fbo.begin();
    {
        if(decoyFramesRemaining > 0){
            decoyGlitchPlayer->play();
            decoyGlitchPlayer->update();
            decoyGlitchPlayer->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            playbackState = ER_PLAYBACK_DECOY;
        }else{
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            playbackState = ER_PLAYBACK_GLITCH;
        }

        float rand = ofRandom(1);
        if(decoyFramesRemaining == 0 && rand < 0.8){
            decoyFramesRemaining = floor(ofRandom(minDecoyFrames, maxDecoyFrames));
        }
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, width, height);
}

string erMediaRenderer::getPlaybackState(){
    return playbackState;
}

void erMediaRenderer::updateDecoyPlayer(){
    if(decoyFramesRemaining > 0){
        decoyFramesRemaining--;
    }else if (decoyFramesRemaining == 0){
        decoyGlitchPlayer->stop();
    }
}