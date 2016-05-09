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

    map<string, ofPtr<erSyncedVideoPlayer>> videoPlayers;
    map<string, vector<string>> texts;
    vector<string> allVideos, audibleVideos, silentVideos, videoCollections;
    vector<string> missingVideos, missingTexts, spacedPathVideos;
    map<string, vector<string>> collectionsToVideos, collectionsToAudibleVideos, collectionsToSilentVideos;
protected:
    void discoverErrors();
    void validateMedia();
    void validateCollectionDir(string path);
    void validateAssetConsistency(const ofFile previewVideo);
    void findMissing(string expectedPath, vector<string>& pushToIfMissing);
    void eraseMedia();
    void loadMedia();
    void loadDirectory(string path);
    void registerVideo(string& collection, const ofFile& video);
    void registerText(const ofFile& liveVideo);
    void registerCollection(string& collection);
    ofDirectory& loadCollectionDir(string path);
    string getRelativePath(const ofFile file);
    string getCollectionName(const ofDirectory directory);
    string liveVideoPathToTextPath(string liveVideoPath);

    ofDirectory productionDir, collectionDir;
    string path, videoPath, textPath, collection;
    erNetwork* network;
    bool hasMediaErrors, mediaIsLoaded;
    float volume;
};