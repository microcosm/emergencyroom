#ifndef emergencyroom_ofxHapPlayerDelayed_h
#define emergencyroom_ofxHapPlayerDelayed_h

#include "ofMain.h"
#include "ofxHapPlayer.h"

class ofxHapPlayerDelayed : public ofxHapPlayer, ofThread{
    int playTime;
    void threadedFunction(){
        int delay = playTime - ofGetElapsedTimeMillis();
        ofSleepMillis(delay);
        play();
    }
    
public:
    void play(int delay=0){
        if(delay <= 0){
            ofxHapPlayer::play();
        }else{
            ofLogVerbose("ofxHapPlayerDelayed") << "play video after " << delay << " ms.";
            playTime = ofGetElapsedTimeMillis()+delay;
            startThread(true);
        }
    }
};

#endif