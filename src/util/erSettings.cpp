#include "erSettings.h"

void erSettings::load(){
    initSettings();
    if(json.open(SETTINGS_FILE)){
        applySettings(json);
    }else{
        erLog("erSettings::load()", "Couldn't open settings file '" + ofToString(SETTINGS_FILE) + "'");
    }
}

void erSettings::toggleDrawing(){
    drawingEnabled = !drawingEnabled;
}

void erSettings::initSettings(){
    fullscreenByDefault = true;
    masterVolume = 1;
    isServer = false;
    isEcg = false;
    ecgPeriod = 1000;
    ecgBpm = 60;
    ecgBeginBeepAt = 0.15;
    ecgEndBeepAt = 0.3;
    drawingEnabled = true;
}

void erSettings::applySettings(ofxJSONElement& json){
    computerName = getComputerName();
    fullscreenByDefault = json[FULLSCREEN_BY_DEFAULT].asBool();
    masterVolume = json[SERVER_MASTER_VOLUME].asFloat();
    isServer = computerName == json[SERVER_MACHINE_NAME].asString();
    isEcg = computerName == json[ECG_MACHINE_NAME].asString();
    ecgBpm = json[ECG_BPM].asFloat();
    ecgBeginBeepAt = json[ECG_BEGIN_BEEP_AT].asFloat();
    ecgEndBeepAt = json[ECG_END_BEEP_AT].asFloat();
    ecgPeriod = 60000 / ecgBpm;
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