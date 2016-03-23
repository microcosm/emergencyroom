#include "erMedia.h"

void erMedia::setup(){
    ensureSymlinkExists();
    loadTestMedia();
    loadProductionMedia();
}

void erMedia::ensureSymlinkExists(){
    if(!ofFile::doesFileExist("dropbox")){
        ofSystem("ln -s ~/Dropbox/ ../../../data/dropbox");
    }
}

void erMedia::loadTestMedia(){
    testSoundPlayer.load(ER_TEST_SOUND);
    testSoundPlayer.setLoop(false);

    testVideoPlayer.load(ER_TEST_VIDEO);
    testVideoPlayer.setLoopState(OF_LOOP_NONE);
}

void erMedia::loadProductionMedia(){
    productionDir = ofDirectory(ER_PRODUCTION_MEDIA_PATH);
    for(auto const& item : productionDir){
        if(item.isDirectory()){
            loadDirectory(item.getAbsolutePath());
        }
    }
}

void erMedia::loadDirectory(string path){
    mediaDir = loadMediaDir(path);
    if(mediaDir.listDir() > 0){
        folder = getBottomLevelFolder(mediaDir);
        registerVideoCollection(folder);
        for(auto const& file : mediaDir){
            registerVideo(folder, file);
        }
    }
}

void erMedia::registerVideo(string& folder, const ofFile file){
    path = getRelativePath(file);
    videoPlayers[path] = ofPtr<erSyncedVideoPlayer>(new erSyncedVideoPlayer);
    videoPlayers[path]->load(file.getAbsolutePath());
    videoPlayers[path]->setLoopState(OF_LOOP_NONE);
    collectionsToVideos[folder].push_back(path);
    allVideos.push_back(path);
}

void erMedia::registerVideoCollection(string& folder){
    videoCollections.push_back(folder);
    vector<string> videos;
    collectionsToVideos[folder] = videos;
}

string erMedia::getRelativePath(const ofFile file){
    vector<string> components = ofSplitString(file.getAbsolutePath(), "/");
    return components.at(components.size() - 2) + "/" + components.at(components.size() - 1);
}

string erMedia::getBottomLevelFolder(const ofDirectory directory){
    vector<string> components = ofSplitString(directory.getAbsolutePath(), "/");
    return components.at(components.size() - 1);
}

ofDirectory& erMedia::loadMediaDir(string path){
    mediaDir = ofDirectory(path);
    for(auto const& ext : ofSplitString(ER_ALLOWED_EXTENSIONS, ",")){
        mediaDir.allowExt(ext);
    }
    return mediaDir;
}