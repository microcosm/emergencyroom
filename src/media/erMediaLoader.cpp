#include "erMediaLoader.h"

void erMediaLoader::setup(erNetwork* _network){
    network = _network;
    ensureMediaSymlinkExists();
    loadTestMedia();
    validateMedia();
    ofAddListener(ofEvents().update, this, &erMediaLoader::update);
}

void erMediaLoader::update(ofEventArgs& args){
    if(network->justBecameClient()){
        loadLiveMedia();
    }else if(network->justBecameServer()){
        loadPreviewMedia();
    }
}

void erMediaLoader::loadLiveMedia(){
    productionDir = ofDirectory(ER_LIVE_MEDIA_PATH);
    loadMedia();
}

void erMediaLoader::loadPreviewMedia(){
    productionDir = ofDirectory(ER_PREVIEW_MEDIA_PATH);
    loadMedia();
}

bool erMediaLoader::hasMissingVideos(){
    return missingVideos.size() > 0;
}

void erMediaLoader::ensureMediaSymlinkExists(){
    if(!ofFile::doesFileExist("dropbox")){
        ofSystem("ln -s ~/Dropbox/ ../../../data/dropbox");
    }
}

void erMediaLoader::validateMedia(){
    productionDir = ofDirectory(ER_PREVIEW_MEDIA_PATH);
    for(auto const& item : productionDir){
        if(item.isDirectory()){
            validateCollectionDir(item.getAbsolutePath());
        }
    }
}

void erMediaLoader::validateCollectionDir(string path){
    collectionDir = loadCollectionDir(path);
    if(collectionDir.listDir() > 0){
        collection = getCollectionName(collectionDir);
        for(auto const& video : collectionDir){
            validateVideo(video);
        }
    }
}

void erMediaLoader::validateVideo(const ofFile video){
    path = video.getAbsolutePath();
    ofStringReplace(path, ER_PREVIEW_DIR, ER_LIVE_DIR);
    ofFile livePath(path);
    if(!livePath.exists()){
        missingVideos.push_back(getRelativePath(livePath));
    }
}

void erMediaLoader::loadTestMedia(){
    testSoundPlayer.load(ER_TEST_SOUND);
    testSoundPlayer.setLoop(false);

    testVideoPlayer.load(ER_TEST_VIDEO);
    testVideoPlayer.setLoopState(OF_LOOP_NONE);
}

void erMediaLoader::eraseMedia(){
    allVideos.clear();
    videoCollections.clear();
    collectionsToVideos.clear();
    videoPlayers.clear();
}

void erMediaLoader::loadMedia(){
    eraseMedia();
    for(auto const& item : productionDir){
        if(item.isDirectory()){
            loadDirectory(item.getAbsolutePath());
        }
    }
}

void erMediaLoader::loadDirectory(string path){
    collectionDir = loadCollectionDir(path);
    if(collectionDir.listDir() > 0){
        collection = getCollectionName(collectionDir);
        registerCollection(collection);
        for(auto const& video : collectionDir){
            registerVideo(collection, video);
        }
    }
}

void erMediaLoader::registerVideo(string& collection, const ofFile video){
    path = getRelativePath(video);
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(video.getAbsolutePath());
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
    collectionsToVideos[collection].push_back(path);
    allVideos.push_back(path);
}

void erMediaLoader::registerCollection(string& collection){
    videoCollections.push_back(collection);
    vector<string> videos;
    collectionsToVideos[collection] = videos;
}

string erMediaLoader::getRelativePath(const ofFile file){
    vector<string> components = ofSplitString(file.getAbsolutePath(), "/");
    return components.at(components.size() - 2) + "/" + components.at(components.size() - 1);
}

string erMediaLoader::getCollectionName(const ofDirectory directory){
    vector<string> components = ofSplitString(directory.getAbsolutePath(), "/");
    return components.at(components.size() - 1);
}

ofDirectory& erMediaLoader::loadCollectionDir(string path){
    collectionDir = ofDirectory(path);
    for(auto const& ext : ofSplitString(ER_ALLOWED_EXTENSIONS, ",")){
        collectionDir.allowExt(ext);
    }
    return collectionDir;
}