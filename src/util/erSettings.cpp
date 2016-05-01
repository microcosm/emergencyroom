#include "erSettings.h"

void erSettings::load(){
    initSettings();
    if(json.open(ER_SETTINGS_FILE)){
        applySettings(json);
    }else{
        erLog("erSettings::load()", "Couldn't open settings file '" + ofToString(ER_SETTINGS_FILE) + "'");
    }
}

void erSettings::toggleServerDrawing(){
    serverDrawingEnabled = !serverDrawingEnabled;
}

void erSettings::toggleClientDrawing(){
    clientDrawingEnabled = !clientDrawingEnabled;
}

void erSettings::initSettings(){
    fullscreenByDefault = false;
    logToFileEnabled = true;
    numChannels = 1;

    masterVolume = 0.95;
    videoVolume = 1;
    ecgVolume = 0.95;
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

    fullscreenByDefault = json[ER_FULLSCREEN_BY_DEFAULT].asBool();
    logToFileEnabled    = json[ER_LOG_TO_FILE_ENABLED].asBool();
    numChannels         = json[ER_NUMBER_OF_VIDEO_CHANNELS].asInt();

    isServer = computerName == json[ER_MACHINE_NAMES][ER_SERVER].asString();
    isEcg    = computerName == json[ER_MACHINE_NAMES][ER_ECG].asString();

    masterVolume =                ofClamp(json[ER_SERVER_VOLUMES][ER_MASTER].asFloat(), 0, 1);
    videoVolume  = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_VIDEO].asFloat(), 0, 1);
    ecgVolume    = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_ECG].asFloat(), 0, 1);
    staticVolume = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_STATIC].asFloat(), 0, 1);

    ecgBpm         = json[ER_ECG_BEEP][ER_BPM].asFloat();
    ecgBeginBeepAt = json[ER_ECG_BEEP][ER_BEGIN_AT].asFloat();
    ecgEndBeepAt   = json[ER_ECG_BEEP][ER_END_AT].asFloat();
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