#include "erMediaRenderer.h"

void erMediaRenderer::setup(){
    glitchStart = glitchEnd = 0;
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

void erMediaRenderer::newGlitchPeriod(unsigned long long from, float duration){
    glitchStart = from;
    glitchEnd = glitchStart + duration;
}

void erMediaRenderer::drawVideo(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(ofGetElapsedTimeMillis() > glitchStart && ofGetElapsedTimeMillis() < glitchEnd){
        ofSetColor(ofColor::white);
        ofDrawBitmapString("GLITCHING NOW", 10, 10);
    }
    if(player->isPlaying()){
        player->draw(x, y, width, height);
    }
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(player->isPlaying()){
        fbo.begin();
        {
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
        }
        fbo.end();

        fboGlitch.draw(fbo, x, y, ofGetWidth(), ofGetHeight());
    }
}