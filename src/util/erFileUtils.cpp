#include "erFileUtils.h"

float erGetVolume(string path){
    int argsIndex = path.find("(s)");
    if(argsIndex != -1){
        return 1;
    }
    argsIndex = path.find("(s=");
    if(argsIndex != -1){
        int start = argsIndex + 3;
        int end = path.find(")", start) - start;
        float volume = ofToFloat(path.substr(start, end));
        return ofClamp(volume, 0, 1);
    }
    return 0;
}

bool erIsFocusVideo(string path){
    int argsIndex = path.find("(focus)");
    if(argsIndex != -1){
        return 1;
    }
}