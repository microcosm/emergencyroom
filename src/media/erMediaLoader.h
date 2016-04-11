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
#define ER_LIVE_DIR "/live/"
#define ER_PREVIEW_DIR "/preview/"
#define ER_ALLOWED_EXTENSIONS "mov,mp4"

class erMediaLoader{

public:
    void setup(erNetwork* _network);
    virtual void update(ofEventArgs& args);
    void setMasterVolume(float _masterVolume);
    void drawErrors();
    void loadLiveMedia();
    void loadPreviewMedia();
    bool hasErrors();

    erSyncedSoundPlayer testSoundPlayer;
    erSyncedVideoPlayer testVideoPlayer;

    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    vector<string> audibleVideos, silentVideos, videoCollections, missingVideos, spacedPathVideos;
    map<string,vector<string>> collectionsToVideos;
protected:
    void ensureMediaSymlinkExists();
    void discoverErrors();
    void validateMedia();
    void validateCollectionDir(string path);
    void validateVideo(const ofFile video);
    void loadTestMedia();
    void eraseMedia();
    void loadMedia();
    void loadDirectory(string path);
    void registerVideo(string& collection, const ofFile video);
    void registerCollection(string& collection);
    ofDirectory& loadCollectionDir(string path);
    string getRelativePath(const ofFile file);
    string getCollectionName(const ofDirectory directory);
    float getVolume(string path);

    ofDirectory productionDir, collectionDir;
    string path, collection;
    erNetwork* network;
    bool hasMediaErrors;
    float volume, masterVolume;
};