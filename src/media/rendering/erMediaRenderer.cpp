#include "erMediaRenderer.h"

void erMediaRenderer::setup(){
    bufferEmpty = true;
    fbo.allocate(ofGetWidth(), ofGetHeight());
    fboGlitch.allocate(ofGetWidth(), ofGetHeight());
    openingGlitchStart = openingGlitchEnd = closingGlitchStart = closingGlitchEnd = 0;
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

void erMediaRenderer::newOpeningGlitchPeriod(u_int64_t from, float duration){
    openingGlitchStart = from;
    openingGlitchEnd = openingGlitchStart + duration;
}

void erMediaRenderer::newClosingGlitchPeriod(u_int64_t from, float duration){
    closingGlitchStart = from;
    closingGlitchEnd = closingGlitchStart + duration;
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    if(player->isPlaying()){
        withinGlitchPeriod() ? drawGlitched(player, x, y, width, height) : drawNormal(player, x, y, width, height);
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
        player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, width, height);
}

bool erMediaRenderer::withinGlitchPeriod(){
    currentTime = ofGetElapsedTimeMillis();
    withinOpeningGlitchPeriod = currentTime > openingGlitchStart && currentTime < openingGlitchEnd;
    withinClosingGlitchPeriod = currentTime > closingGlitchStart && currentTime < closingGlitchEnd;
    return withinOpeningGlitchPeriod || withinClosingGlitchPeriod;
}