#include "erSettings.h"

void erSettings::load(){
    initSettings();
    if(json.open(SETTINGS_FILE)){
        applySettings(json);
    }else{
        erLog("erSettings::load()", "Couldn't open settings file '" + ofToString(SETTINGS_FILE) + "'");
    }
}

void erSettings::toggleServerDrawing(){
    serverDrawingEnabled = !serverDrawingEnabled;
}

void erSettings::toggleClientDrawing(){
    clientDrawingEnabled = !clientDrawingEnabled;
}

void erSettings::initSettings(){
    fullscreenByDefault = true;
    logToFileEnabled = true;
    numChannels = 1;
    masterVolume = 1;
    videoVolume = 1;
    ecgVolume = 1;
    staticVolume = 1;
    isServer = false;
    isEcg = false;
    ecgPeriod = 1000;
    ecgBpm = 60;
    ecgBeginBeepAt = 0.15;
    ecgEndBeepAt = 0.3;
    serverDrawingEnabled = true;
    clientDrawingEnabled = true;
}

void erSettings::applySettings(ofxJSONElement& json){
    computerName = getComputerName();
    fullscreenByDefault = json[FULLSCREEN_BY_DEFAULT].asBool();
    logToFileEnabled = json[LOG_TO_FILE_ENABLED].asBool();
    numChannels = json[NUMBER_OF_VIDEO_CHANNELS].asInt();
    masterVolume = ofClamp(json[SERVER_MASTER_VOLUME].asFloat(), 0, 1);
    videoVolume = ofClamp(json[SERVER_VIDEO_VOLUME].asFloat(), 0, 1);
    ecgVolume = masterVolume * ofClamp(json[SERVER_ECG_VOLUME].asFloat(), 0, 1);
    staticVolume = masterVolume * ofClamp(json[SERVER_STATIC_VOLUME].asFloat(), 0, 1);
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