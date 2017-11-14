#ifdef __linux__
#include "erOmxPlayer.h"

void erOmxPlayer::prepareToPlay(string absolutePath){
    isPlaying = false;
    loadMovie(absolutePath);
    setPaused(true);
}

void erOmxPlayer::startPlaying(){
    isPlaying = true;
    restartMovie();
    setPaused(false);
}

void erOmxPlayer::update(){
    if(getCurrentFrame() >= getTotalNumFrames()){
        isPlaying = false;
    }
}

bool erOmxPlayer::isActuallyPlaying(){
    return isPlaying;
}
#endif
