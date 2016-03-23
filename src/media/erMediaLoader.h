#pragma once
#include "ofMain.h"
#include "erNetwork.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"
#include "erMediaRenderer.h"

#define ER_TEST_SOUND "test/audio.mp3"
#define ER_TEST_VIDEO "test/fingers.mov"
#define ER_TEST_HAP "test/video.mov"
#define ER_LIVE_MEDIA_PATH "dropbox/ER-Media/live/"
#define ER_PREVIEW_MEDIA_PATH "dropbox/ER-Media/preview/"
#define ER_ALLOWED_EXTENSIONS "mov,mp4"

class erMediaLoader{

public:
    void setup(erNetwork* _network);
    virtual void update(ofEventArgs& args);
    void loadLiveMedia();
    void loadPreviewMedia();

    erSyncedSoundPlayer testSoundPlayer;
    erSyncedVideoPlayer testVideoPlayer;

    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    vector<string> allVideos, videoCollections;
    map<string,vector<string>> collectionsToVideos;
protected:
    void ensureSymlinkExists();
    void loadTestMedia();
    void eraseMedia();
    void loadMedia();
    void loadDirectory(string path);
    void registerVideo(string& folder, const ofFile file);
    void registerVideoCollection(string& folder);
    ofDirectory& loadMediaDir(string path);
    string getRelativePath(const ofFile file);
    string getBottomLevelFolder(const ofDirectory file);

    ofDirectory productionDir, mediaDir;
    string path, folder;
    erNetwork* network;
};