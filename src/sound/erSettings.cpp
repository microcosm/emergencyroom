#include "erSettings.h"

void erSettings::load(){
    initSettings();
    if(json.open(SETTINGS_FILE)){
        applySettings(json);
    }else{
        erLog("erSettings::load()", "Couldn't open settings file '" + ofToString(SETTINGS_FILE) + "'");
    }
}

void erSettings::initSettings(){
    fullscreenByDefault = true;
    masterVolume = 1;
    isServer = false;
}

void erSettings::applySettings(ofxJSONElement& json){
    fullscreenByDefault = json[FULLSCREEN_BY_DEFAULT].asBool();
    masterVolume = json[SERVER_MASTER_VOLUME].asFloat();
    isServer = getComputerName() == json[SERVER_MACHINE_NAME].asString();
}

string erSettings::getComputerName(){
    fp = popen("scutil --get ComputerName", "r");
    if(fp != NULL){
        fgets(path, 20, fp);
        string str(path);
        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
        pclose(fp);
        return str;
    }
    return "";
}