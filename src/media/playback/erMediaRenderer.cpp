#include "erMediaRenderer.h"

void erMediaRenderer::drawVideo(erSyncedVideoPlayer* player, int x, int y, int width, int height){
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