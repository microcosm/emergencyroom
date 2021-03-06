#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "erUtils.h"
#include "erSoundPlayer.h"
#include "erVideoPlayer.h"
#include "erOmxManager.h"

#define FOCUS_COLLECTION "FOCUS"

class erMediaLoader{

public:
    void setup(erOmxManager* _omxManager);
    void drawErrors();
    void loadLiveMedia();
    void loadPreviewMedia();
    bool hasErrors();
    bool isLoaded();

    map<string, ofPtr<erVideoPlayer>> videoPlayers;
    vector<string> allVideos, audibleVideos, silentVideos, videoCollections;
    vector<string> missingVideos, spacedPathVideos, focusVideos;
    map<string, vector<string>> collectionsToVideos, collectionsToAudibleVideos, collectionsToSilentVideos;
protected:
    void discoverErrors();
    void validateMedia();
    void validateCollectionDir(string path);
    void validateAssetConsistency(const ofFile previewVideo);
    void findMissing(string expectedPath, vector<string>& pushToIfMissing);
    void loadMedia();
    void loadDirectory(string path);
    void registerVideo(string& collection, const ofFile& video);
    void registerCollection(string& collection);
    ofDirectory& loadCollectionDir(string path);
    string getRelativePath(const ofFile file);
    string getCollectionName(const ofDirectory directory);

    ofDirectory productionDir, collectionDir;
    string path, videoPath, collection;
    bool hasMediaErrors, mediaIsLoaded;
    float volume;

    erOmxManager* omxManager;
};
