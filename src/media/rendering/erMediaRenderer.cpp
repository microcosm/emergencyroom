#include "erMediaRenderer.h"

void erMediaRenderer::setup(erNetwork* _network){
    erGlitchTimer::setup();
    network = _network;
    //decoyAssigned = false;
    bufferEmpty = true;
    //decoyFramesRemaining = 0;
    //minDecoyFrames = 3;
    //maxDecoyFrames = 7;
    fboWidth = ofGetWidth() * 0.3;
    fboHeight = ofGetHeight() * 0.3;
    fbo.allocate(fboWidth, fboHeight);
    fboGlitch.allocate(fboWidth, fboHeight);
    fboGlitch.setGlichResetProbability(0.2);
}

void erMediaRenderer::update(){
    for(auto const& player : *videoPlayers){
        player.second->checkSchedule();
        if(player.second->isCurrentlyPlaying()){
            player.second->update();
        }
    }
}

void erMediaRenderer::setVideoPlayers(map<string, ofPtr<erSyncedVideoPlayer>>* _videoPlayers){
    videoPlayers = _videoPlayers;
}

void erMediaRenderer::assignDecoyGlitch(erSyncedVideoPlayer* _videoPlayer){
    /*stopDecoyPlayer();
    decoyGlitchPlayer = _videoPlayer;
    decoyAssigned = true;*/
}

void erMediaRenderer::draw(erSyncedVideoPlayer* player, int x, int y, int width, int height, int channel){
    bool isPlaying = player->isCurrentlyPlaying();
    if(isPlaying){
        withinGlitchPeriod(channel) && settings.isClient ?
            drawGlitched(player, x, y, width, height) :
            drawNormal(player, x, y, width, height);
    }else if(settings.isServer){
        ofDrawBitmapString("Scheduled...", x + 10, y + 20);// + player->getPath(), x + 10, y + 20);
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
    if(settings.isServer){
        ofDrawBitmapString("Playing", x + 10, y + 20);// + player->getPath(), x + 10, y + 20);
    }
    playbackState = ER_PLAYBACK_NORMAL;
}

void erMediaRenderer::drawGlitched(erSyncedVideoPlayer* player, int x, int y, int width, int height){
    fbo.begin();
    {
        float playerDuration = player->getDuration();

        /*if(player != decoyGlitchPlayer && playerDuration > settings.minDecoyDuration && decoyFramesRemaining > 0){
            //https://github.com/openframeworks/openFrameworks/issues/4485
            if(!decoyGlitchPlayer->isCurrentlyPlaying()){
                decoyGlitchPlayer->setPosition(0);
                decoyGlitchPlayer->play();
            }else{
                decoyGlitchPlayer->update();
                decoyGlitchPlayer->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            }
            decoyFramesRemaining--;
            playbackState = ER_PLAYBACK_DECOY;
        }else{*/
            player->draw(0, 0, fbo.getWidth(), fbo.getHeight());
            playbackState = ER_PLAYBACK_GLITCH;
        /*}

        float rand = ofRandom(1);
        if(decoyFramesRemaining == 0 && rand < 0.5){
            decoyFramesRemaining = floor(ofRandom(minDecoyFrames, maxDecoyFrames));
        }*/
    }
    fbo.end();
    fboGlitch.draw(fbo, x, y, width, height);
}

string erMediaRenderer::getPlaybackState(){
    return playbackState;
}

void erMediaRenderer::stopDecoyPlayer(){
    /*if(decoyGlitchPlayer != NULL && decoyAssigned){
        if(decoyGlitchPlayer->isCurrentlyPlaying()){
            decoyGlitchPlayer->stop();
        }
    }*/
}
