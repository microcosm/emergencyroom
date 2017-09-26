#pragma once
#include "ofMain.h"
#include "erSettings.h"
#include "erNetwork.h"
#include "erFileUtils.h"
#include "erSoundPlayer.h"
#include "erVideoPlayer_.h"

class erMediaLoader{

public:
    void setup(erNetwork* _network);
    void update();
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
    erNetwork* network;
    bool hasMediaErrors, mediaIsLoaded;
    float volume;
};
