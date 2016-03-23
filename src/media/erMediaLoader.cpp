#include "erMediaLoader.h"

void erMediaLoader::setup(erNetwork* _network){
    network = _network;
    ensureSymlinkExists();
    loadTestMedia();
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

void erMediaLoader::ensureSymlinkExists(){
    if(!ofFile::doesFileExist("dropbox")){
        ofSystem("ln -s ~/Dropbox/ ../../../data/dropbox");
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
    mediaDir = loadMediaDir(path);
    if(mediaDir.listDir() > 0){
        folder = getBottomLevelFolder(mediaDir);
        registerVideoCollection(folder);
        for(auto const& file : mediaDir){
            registerVideo(folder, file);
        }
    }
}

void erMediaLoader::registerVideo(string& folder, const ofFile file){
    path = getRelativePath(file);
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(file.getAbsolutePath());
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
    collectionsToVideos[folder].push_back(path);
    allVideos.push_back(path);
}

void erMediaLoader::registerVideoCollection(string& folder){
    videoCollections.push_back(folder);
    vector<string> videos;
    collectionsToVideos[folder] = videos;
}

string erMediaLoader::getRelativePath(const ofFile file){
    vector<string> components = ofSplitString(file.getAbsolutePath(), "/");
    return components.at(components.size() - 2) + "/" + components.at(components.size() - 1);
}

string erMediaLoader::getBottomLevelFolder(const ofDirectory directory){
    vector<string> components = ofSplitString(directory.getAbsolutePath(), "/");
    return components.at(components.size() - 1);
}

ofDirectory& erMediaLoader::loadMediaDir(string path){
    mediaDir = ofDirectory(path);
    for(auto const& ext : ofSplitString(ER_ALLOWED_EXTENSIONS, ",")){
        mediaDir.allowExt(ext);
    }
    return mediaDir;
}