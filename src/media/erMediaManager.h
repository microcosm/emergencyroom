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
#define ER_ALLOWED_EXTENSIONS "mov"

class erMediaManager{

public:
    void setup();
    void play(erPlayParams params);
    virtual void update(ofEventArgs& args);
    virtual void draw(ofEventArgs& args);
    vector<string> getVideoCollections();
    map<string,vector<string>> getCollectionsToVideos();
protected:
    void ensureSymlinkExists();
    void loadTestMedia();
    void loadProductionMedia();
    void loadDirectory(string path);
    void registerVideo(string& folder, const ofFile file);
    void registerVideoCollection(string& folder);
    string getRelativePath(const ofFile file);
    string getBottomLevelFolder(const ofDirectory file);
    erSyncedSoundPlayer testSoundPlayer;
    erSyncedVideoPlayer testVideoPlayer;
    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    vector<string> videoCollections;
    map<string,vector<string>> collectionsToVideos;
    ofDirectory mediaDir, subDir;
    string path, folder;
};