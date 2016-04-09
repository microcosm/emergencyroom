#include "erMediaLoader.h"

void erMediaLoader::setup(erNetwork* _network){
    network = _network;
    ensureMediaSymlinkExists();
    loadTestMedia();
    validateMedia();
    discoverErrors();
    ofAddListener(ofEvents().update, this, &erMediaLoader::update);
}

void erMediaLoader::update(ofEventArgs& args){
    if(network->justBecameClient()){
        loadLiveMedia();
    }else if(network->justBecameServer()){
        loadPreviewMedia();
    }
}

void erMediaLoader::drawErrors(){
    ofSetColor(ofColor::black);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor::white);
    int i = 1;
    ofDrawBitmapString("Has spaces in filename", 20, 30);
    for(const auto video : spacedPathVideos){
        ofDrawBitmapString(video, 20, 30 + 20 * i++);
    }
    ofDrawBitmapString("Missing from live folder", 20, 60 + 20 * i++);
    for(const auto video : missingVideos){
        ofDrawBitmapString(video, 20, 60 + 20 * i++);
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

bool erMediaLoader::hasErrors(){
    return hasMediaErrors;
}

void erMediaLoader::ensureMediaSymlinkExists(){
    if(!ofFile::doesFileExist("dropbox")){
        ofSystem("ln -s ~/Dropbox/ ../../../data/dropbox");
    }
}

void erMediaLoader::discoverErrors(){
    hasMediaErrors = missingVideos.size() > 0 || spacedPathVideos.size() > 0;
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
    if(path.find(" ") != -1){
        spacedPathVideos.push_back(getRelativePath(video));
    }
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
    audibleVideos.clear();
    silentVideos.clear();
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
    int volume = getVolume(path);
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(video.getAbsolutePath());
    videoPlayers[path]->setVolume(volume);
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
    collectionsToVideos[collection].push_back(path);
    volume == 0 ? silentVideos.push_back(path) : audibleVideos.push_back(path);
}

void erMediaLoader::registerCollection(string& collection){
    videoCollections.push_back(collection);
    vector<string> videos;
    collectionsToVideos[collection] = videos;
}

ofDirectory& erMediaLoader::loadCollectionDir(string path){
    collectionDir = ofDirectory(path);
    for(auto const& ext : ofSplitString(ER_ALLOWED_EXTENSIONS, ",")){
        collectionDir.allowExt(ext);
    }
    return collectionDir;
}

string erMediaLoader::getRelativePath(const ofFile file){
    vector<string> components = ofSplitString(file.getAbsolutePath(), "/");
    return components.at(components.size() - 2) + "/" + components.at(components.size() - 1);
}

string erMediaLoader::getCollectionName(const ofDirectory directory){
    vector<string> components = ofSplitString(directory.getAbsolutePath(), "/");
    return components.at(components.size() - 1);
}

int erMediaLoader::getVolume(string path){
    return path.find("(s)") == -1 ? 0 : 1;
}