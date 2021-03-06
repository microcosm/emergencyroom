#include "erMediaLoader.h"

void erMediaLoader::setup(erOmxManager* _omxManager){
    omxManager = _omxManager;
    mediaIsLoaded = false;
    validateMedia();
    discoverErrors();
    if(settings.isServer){
        loadPreviewMedia();
    } else {
        loadLiveMedia();
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
    productionDir = ofDirectory(settings.liveMediaPath);
    loadMedia();
}

void erMediaLoader::loadPreviewMedia(){
    productionDir = ofDirectory(settings.previewMediaPath);
    loadMedia();
}

bool erMediaLoader::hasErrors(){
    return hasMediaErrors;
}

bool erMediaLoader::isLoaded(){
    return mediaIsLoaded;
}

void erMediaLoader::discoverErrors(){
    hasMediaErrors = missingVideos.size() > 0 || spacedPathVideos.size() > 0;
}

void erMediaLoader::validateMedia(){
    productionDir = ofDirectory(settings.previewMediaPath);
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
            validateAssetConsistency(video);
        }
    }
}

void erMediaLoader::validateAssetConsistency(const ofFile previewVideo){
    path = previewVideo.getAbsolutePath();
    if(path.find(" ") != -1){
        spacedPathVideos.push_back(getRelativePath(previewVideo));
    }
    ofStringReplace(path, settings.previewMediaDir, settings.liveMediaDir);
    findMissing(path, missingVideos);
}

void erMediaLoader::findMissing(string expectedPath, vector<string>& pushToIfMissing){
    ofFile targetPath(path);
    if(!targetPath.exists()){
        pushToIfMissing.push_back(getRelativePath(targetPath));
    }
}

void erMediaLoader::loadMedia(){
    for(auto const& item : productionDir){
        if(item.isDirectory()){
            loadDirectory(item.getAbsolutePath());
        }
    }
    mediaIsLoaded = true;
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

void erMediaLoader::registerVideo(string& collection, const ofFile& video){
    path = getRelativePath(video);
    volume = erGetVolume(path);

    videoPlayers[path] = ofPtr<erVideoPlayer>(new erVideoPlayer);
    videoPlayers[path]->setup(video.getAbsolutePath(), path, 0, OF_LOOP_NONE, omxManager);

    allVideos.push_back(path);

    if(collection == FOCUS_COLLECTION){
        focusVideos.push_back(path);
        audibleVideos.push_back(path);
    }else{
        collectionsToVideos[collection].push_back(path);

        if(volume == 0){
            silentVideos.push_back(path);
            collectionsToSilentVideos[collection].push_back(path);
        }else{
            audibleVideos.push_back(path);
            collectionsToAudibleVideos[collection].push_back(path);
        }
    }
}

void erMediaLoader::registerCollection(string& collection){
    if(collection != FOCUS_COLLECTION){
        videoCollections.push_back(collection);
        vector<string> videos;
        collectionsToVideos[collection] = videos;
        collectionsToSilentVideos[collection] = videos;
        collectionsToAudibleVideos[collection] = videos;
    }
}

ofDirectory& erMediaLoader::loadCollectionDir(string path){
    collectionDir = ofDirectory(path);
    collectionDir.allowExt(settings.videoFileExtension);
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
