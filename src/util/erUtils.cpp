#include "erUtils.h"

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

bool erContains(string& needle, vector<string>& haystack){
    return find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

void erShuffle(vector<int>& toShuffle){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle (toShuffle.begin(), toShuffle.end(), default_random_engine(seed));
}

void erThrowInvalidPath(string method){
    throw invalid_argument("This path should never be reached. Path: " + method);
}
