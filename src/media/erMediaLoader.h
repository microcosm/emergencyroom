#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "erFileUtils.h"
#include "erSyncedSoundPlayer.h"
#include "erSyncedVideoPlayer.h"

class erMediaLoader{

public:
    void setup(erNetwork* _network);
    virtual void update(ofEventArgs& args);
    void drawErrors();
    void loadLiveMedia();
    void loadPreviewMedia();
    bool hasErrors();
    bool isLoaded();

    erSyncedSoundPlayer testSoundPlayer;
    erSyncedVideoPlayer testVideoPlayer;

    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    vector<string> audibleVideos, silentVideos, videoCollections, missingVideos, missingTexts, spacedPathVideos;
    map<string,vector<string>> collectionsToVideos;
protected:
    void discoverErrors();
    void validateMedia();
    void validateCollectionDir(string path);
    void validateAssetConsistency(const ofFile previewVideo);
    void findMissing(string expectedPath, vector<string>& pushToIfMissing);
    void loadTestMedia();
    void eraseMedia();
    void loadMedia();
    void loadDirectory(string path);
    void registerVideo(string& collection, const ofFile video);
    void registerCollection(string& collection);
    ofDirectory& loadCollectionDir(string path);
    string getRelativePath(const ofFile file);
    string getCollectionName(const ofDirectory directory);

    ofDirectory productionDir, collectionDir;
    string path, collection;
    erNetwork* network;
    bool hasMediaErrors, mediaIsLoaded;
    float volume;
};