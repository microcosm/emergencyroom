#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network){
    erGlitchTimer::setup();
    network = _network;
    decoyAssigned = false;
    bufferEmpty = true;
    decoyFramesRemaining = 0;
    minDecoyFrames = 3;
    maxDecoyFrames = 7;
    fboWidth = ofGetWidth() * 0.3;
    fboHeight = ofGetHeight() * 0.3;
    fbo.allocate(fboWidth, fboHeight);
    fboGlitch.allocate(fboWidth, fboHeight);
    fboGlitch.setGlichResetProbability(0.2);
    ofAddListener(ofEvents().update, this, &erMediaRenderer::update);
}

void erMediaRenderer::update(ofEventArgs& args){
    for(auto const& player : *videoPlayers){
        if(player.second->isPlaying()){
            player.second->lock();
            player.second->update();
            player.second->unlock();
        }
    }
}

void erMediaRenderer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
}

void erMediaRenderer::assignDecoyGlitch(erSyncedVideoPlayer* _videoPlayer){
    stopDecoyPlayer();
    decoyGlitchPlayer = _videoPlayer;
    decoyAssigned = true;
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height, int channel){
    player->lock();
    bool isPlaying = player->isPlaying();
    player->unlock();
    if(isPlaying){
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
    player->lock();
    player->draw(x, y, width, height);
    player->unlock();
    playbackState = ER_PLAYBACK_NORMAL;
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    fbo.begin();
    {
        player->lock();
        if(player->getDuration() > settings.minDecoyDuration && decoyFramesRemaining > 0){
            decoyGlitchPlayer->lock();
            if(!decoyGlitchPlayer->isPlaying()) decoyGlitchPlayer->play();
            decoyGlitchPlayer->update();
            decoyGlitchPlayer->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            decoyGlitchPlayer->unlock();
            decoyFramesRemaining--;
            playbackState = ER_PLAYBACK_DECOY;
        }else{
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            playbackState = ER_PLAYBACK_GLITCH;
        }
        player->unlock();

        float rand = ofRandom(1);
        if(decoyFramesRemaining == 0 && rand < 0.5){
            decoyFramesRemaining = floor(ofRandom(minDecoyFrames, maxDecoyFrames));
        }
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, width, height);
}

string erMediaRenderer::getPlaybackState(){
    return playbackState;
}

void erMediaRenderer::stopDecoyPlayer(){
    if(decoyGlitchPlayer != NULL && decoyAssigned){
        decoyGlitchPlayer->lock();
        if(decoyGlitchPlayer->isPlaying()){
            decoyGlitchPlayer->stop();
        }
        decoyGlitchPlayer->unlock();
    }
}