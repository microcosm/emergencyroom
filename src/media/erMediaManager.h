#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erSyncedHapPlayer.h"

#define ER_TEST_SOUND "test/audio.mp3"
#define ER_TEST_VIDEO "test/fingers.mov"
#define ER_TEST_HAP "test/video.mov"

class erMediaManager{

public:
    void setup();
    void play(erPlayParams params);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
protected:
    void loadTestVideos();
    vector<ofPtr<erSyncedSoundPlayer>> soundPlayers;
    vector<ofPtr<erSyncedVideoPlayer>> videoPlayers;
};