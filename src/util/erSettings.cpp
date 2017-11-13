#include "erSettings.h"

void erSettings::load(){
    initSettings();
    if(json.open(ER_SETTINGS_FILE)){
        applySettings(json);
    }else{
        erLog("erSettings::load()", "Couldn't open settings file '" + ofToString(ER_SETTINGS_FILE) + "'");
    }
}

void erSettings::incrementServerViewMode(){
    if(serverViewMode == erChannelsView){
        serverViewMode = erEcgView;
    }else if(serverViewMode == erEcgView){
        serverViewMode = erAudioView;
    }else if(serverViewMode == erAudioView){
        serverViewMode = erChannelsView;
    }
}

void erSettings::incrementClientViewMode(){
    if(clientViewMode == erVideoView){
        clientViewMode = erStatusView;
    }else if(clientViewMode == erStatusView){
        clientViewMode = erVideoView;
    }
}

bool erSettings::renderChannels(){
    return serverViewMode == erChannelsView;
}

bool erSettings::renderEcg(){
    return serverViewMode == erEcgView;
}

bool erSettings::renderAudioUI(){
    return serverViewMode == erAudioView;
}

bool erSettings::renderVideo(){
    return clientViewMode == erVideoView;
}

bool erSettings::renderStatus(){
    return clientViewMode == erStatusView;
}

void erSettings::initSettings(){
    ofPort = 0;
    managePort = 1;

    serverIP = "";

    doManageClients = false;

    fullscreenServer = false;
    fullscreenClient = false;
    serverViewMode = erChannelsView;
    clientViewMode = erStatusView;
    fontPath = "";

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
}

void erSettings::applySettings(ofxJSONElement& json){
    computerName = getComputerName();

    ofPort = json[ER_OF_PORT].asInt();
    managePort = json[ER_MANAGE_PORT].asInt();

    serverIP = json[ER_SERVER_IP].asString();
    for(int i = 0; i < json[ER_CLIENT_IPS].size(); i++){
        clientIPs.push_back(json[ER_CLIENT_IPS][i].asString());
    }

    doManageClients = json[ER_DO_MANAGE_CLIENTS].asBool();

    fullscreenServer = json[ER_FULLSCREEN_SERVER].asBool();
    fullscreenClient = json[ER_FULLSCREEN_CLIENT].asBool();
    serverViewMode = getServerViewMode();
    clientViewMode = getClientViewMode();
    fontPath = json[ER_FONT_PATH].asString();

    logToFileEnabled    = json[ER_LOG_TO_FILE_ENABLED].asBool();
    testContentMode     = json[ER_TEST_CONTENT_MODE].asBool();
    numChannels         = json[ER_NUMBER_OF_VIDEO_CHANNELS].asInt();
    minDecoyDuration    = json[ER_MIN_DECOY_DURATION].asFloat();

    isServer = computerName == json[ER_MACHINE_NAMES][ER_SERVER].asString();
    isClient = !isServer;

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

erServerViewMode erSettings::getServerViewMode(){
    string mode = json[ER_SERVER_VIEW_MODE].asString();
    if(mode == "channels"){
        return erChannelsView;
    }else if(mode == "ecg"){
        return erEcgView;
    }else if(mode == "audio"){
        return erAudioView;
    }else{
        return serverViewMode;
    }
}

erClientViewMode erSettings::getClientViewMode(){
    string mode = json[ER_CLIENT_VIEW_MODE].asString();
    if(mode == "video"){
        return erVideoView;
    }else if(mode == "status"){
        return erStatusView;
    }else{
        return clientViewMode;
    }
}
