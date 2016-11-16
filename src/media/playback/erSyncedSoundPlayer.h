#ifdef __APPLE__
    #pragma once
    #include "ofMain.h"
    #include "erSyncedMediaPlayer.h"

    class erSyncedSoundPlayer : public ofSoundPlayer, public erSyncedMediaPlayer{

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
#endif
