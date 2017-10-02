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
    serverIP = "";
    serverPort = 0;

    fullscreenServer = false;
    fullscreenClient = false;

    logToFileEnabled = true;
    testContentMode = false;
    numChannels = 1;
    minDecoyDuration = 1;

    masterVolume = 0.95;
    breathingVolume = 1;
    videoVolume = 1;
    ecgVolume = 0.95;
    staticVolume = 1;

    isServer = false;
    isClient = false;
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
    videoFileExtension = "";

    shortestSequenceDelay = 500;
    longestSequenceDelay = 1000;

    serverDrawingEnabled = true;
    clientDrawingEnabled = true;
}

void erSettings::applySettings(ofxJSONElement& json){
    computerName = getComputerName();

    serverIP = json[ER_SERVER_IP].asString();
    serverPort = json[ER_SERVER_PORT].asInt();

    fullscreenServer = json[ER_FULLSCREEN_SERVER].asBool();
    fullscreenClient = json[ER_FULLSCREEN_CLIENT].asBool();

    logToFileEnabled    = json[ER_LOG_TO_FILE_ENABLED].asBool();
    testContentMode     = json[ER_TEST_CONTENT_MODE].asBool();
    numChannels         = json[ER_NUMBER_OF_VIDEO_CHANNELS].asInt();
    minDecoyDuration    = json[ER_MIN_DECOY_DURATION].asFloat();

    isServer = computerName == json[ER_MACHINE_NAMES][ER_SERVER].asString();
    isClient = !isServer;
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

    shortestSequenceDelay = json[ER_MEDIA_SEQUENCING][ER_SHORTEST_DELAY].asInt();
    longestSequenceDelay  = json[ER_MEDIA_SEQUENCING][ER_LONGEST_DELAY].asInt();

    liveMediaPath      = json[ER_MEDIA_FILES][testContentMode ? ER_TEST_CONTENT_LIVE_PATH : ER_LIVE_PATH].asString();
    previewMediaPath   = json[ER_MEDIA_FILES][testContentMode ? ER_TEST_CONTENT_PREVIEW_PATH : ER_PREVIEW_PATH].asString();
    breathingSoundPath = json[ER_MEDIA_FILES][ER_BREATHING_SOUND_PATH].asString();

    liveMediaDir       = json[ER_MEDIA_FILES][ER_LIVE_DIR].asString();
    previewMediaDir    = json[ER_MEDIA_FILES][ER_PREVIEW_DIR].asString();
    videoFileExtension = json[ER_MEDIA_FILES][ER_VIDEO_FILE_EXTENSION].asString();
}

string erSettings::getComputerName(){
#ifdef __APPLE__
    fp = popen("scutil --get HostName", "r");
    if(fp != NULL){
        fgets(path, 20, fp);
        string str(path);
        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
        pclose(fp);
        return str;
    }
#endif
    return "";
}
