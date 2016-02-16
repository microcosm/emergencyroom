#pragma once
#include "ofMain.h"
#include "erPlayParams.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erSyncedHapPlayer.h"

#define ER_TEST_SOUND "test/audio.mp3"
#define ER_TEST_VIDEO "test/fingers.mov"
#define ER_TEST_HAP "test/video.mov"
#define ER_PRODUCTION_MEDIA_PATH "dropbox/ER-Media/"
#define ER_ALLOWED_EXTENSIONS "avi"

class erMediaManager{

public:
    void setup();
    void play(erPlayParams params);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
protected:
    void loadTestMedia();
    void loadProductionMedia();
    void loadDirectory(string path);
    void pushVideo(const ofFile file);
    erSyncedSoundPlayer testSoundPlayer;
    erSyncedVideoPlayer testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    ofDirectory mediaDir, subDir;
    string path;
};