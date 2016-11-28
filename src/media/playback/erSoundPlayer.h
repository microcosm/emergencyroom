#pragma once
#include "ofMain.h"
#include "erThreadedPlayer.h"

class erSoundPlayer : public ofSoundPlayer, public erThreadedPlayer{

public:
    bool isOrWillBePlaying(){
        return scheduled || isPlaying();
    }

protected:
    void beginPlayback(){
        setSpeed(params.getSpeed());
        play();
    }
};
