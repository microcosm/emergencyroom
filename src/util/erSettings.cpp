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
    minDecoyDuration = 1;

    masterVolume = 0.95;
    breathingVolume = 1;
    videoVolume = 1;
    ecgVolume = 0.95;
    staticVolume = 1;

    isServer = false;
    isEcg = false;

    ecgHighestBpm = 60;
    ecgLowestBpm = 30;
    ecgBpmPeriodSecs = 1;
    ecgBeginBeepAt = 0.15;
    ecgEndBeepAt = 0.3;

    liveMediaPath = "";
    previewMediaPath = "";
    breathingSoundPath = "";
    liveMediaDir = "";
    previewMediaDir = "";
    textMediaDir = "";
    videoFileExtension = "";
    textFileExtension = "";

    serverDrawingEnabled = true;
    clientDrawingEnabled = true;
}

void erSettings::applySettings(ofxJSONElement& json){
    computerName = getComputerName();

    fullscreenByDefault = json[ER_FULLSCREEN_BY_DEFAULT].asBool();
    logToFileEnabled    = json[ER_LOG_TO_FILE_ENABLED].asBool();
    numChannels         = json[ER_NUMBER_OF_VIDEO_CHANNELS].asInt();
    minDecoyDuration    = json[ER_MIN_DECOY_DURATION].asFloat();

    isServer = computerName == json[ER_MACHINE_NAMES][ER_SERVER].asString();
    isEcg    = computerName == json[ER_MACHINE_NAMES][ER_ECG].asString();

    masterVolume    =                ofClamp(json[ER_SERVER_VOLUMES][ER_MASTER].asFloat(), 0, 1);
    breathingVolume = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_BREATHING].asFloat(), 0, 1);
    videoVolume     = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_VIDEO].asFloat(), 0, 1);
    ecgVolume       = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_ECG].asFloat(), 0, 1);
    staticVolume    = masterVolume * ofClamp(json[ER_SERVER_VOLUMES][ER_STATIC].asFloat(), 0, 1);

    ecgHighestBpm    = json[ER_ECG_BEEP][ER_HIGHEST_BPM].asFloat();
    ecgLowestBpm     = json[ER_ECG_BEEP][ER_LOWEST_BPM].asFloat();
    ecgBpmPeriodSecs = json[ER_ECG_BEEP][ER_BPM_PERIOD_SECS].asInt();
    ecgBeginBeepAt   = json[ER_ECG_BEEP][ER_BEGIN_AT].asFloat();
    ecgEndBeepAt     = json[ER_ECG_BEEP][ER_END_AT].asFloat();

    liveMediaPath      = json[ER_MEDIA_FILES][ER_LIVE_PATH].asString();
    previewMediaPath   = json[ER_MEDIA_FILES][ER_PREVIEW_PATH].asString();
    breathingSoundPath = json[ER_MEDIA_FILES][ER_BREATHING_SOUND_PATH].asString();
    liveMediaDir       = json[ER_MEDIA_FILES][ER_LIVE_DIR].asString();
    previewMediaDir    = json[ER_MEDIA_FILES][ER_PREVIEW_DIR].asString();
    textMediaDir       = json[ER_MEDIA_FILES][ER_TEXT_DIR].asString();
    videoFileExtension = json[ER_MEDIA_FILES][ER_VIDEO_FILE_EXTENSION].asString();
    textFileExtension  = json[ER_MEDIA_FILES][ER_TEXT_FILE_EXTENSION].asString();
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