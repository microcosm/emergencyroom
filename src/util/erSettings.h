#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erLogger.h"
#include "ofxJSON.h"

#define SETTINGS_FILE "settings.json"
#define FULLSCREEN_BY_DEFAULT "fullscreen-by-default"
#define LOG_TO_FILE_ENABLED "enable-log-to-file"
#define NUMBER_OF_VIDEO_CHANNELS "number-of-video-channels"
#define SERVER_MASTER_VOLUME "server-master-volume"
#define SERVER_VIDEO_VOLUME "server-video-volume"
#define SERVER_ECG_VOLUME "server-ecg-volume"
#define SERVER_STATIC_VOLUME "server-static-volume"
#define SERVER_MACHINE_NAME "server-machine-name"
#define ECG_MACHINE_NAME "ecg-machine-name"
#define ECG_BPM "ecg-bpm"
#define ECG_BEGIN_BEEP_AT "ecg-begin-beep-at"
#define ECG_END_BEEP_AT "ecg-end-beep-at"

class erSettings{

public:
    void load();
    void toggleDrawing();
    float masterVolume;
    float videoVolume;
    float ecgVolume;
    float staticVolume;
    bool fullscreenByDefault;
    bool logToFileEnabled;
    int numChannels;
    bool isServer;
    bool isEcg;
    float ecgPeriod;
    float ecgBpm;
    float ecgBeginBeepAt;
    float ecgEndBeepAt;
    bool drawingEnabled;

protected:
    void initSettings();
    void applySettings(ofxJSONElement& json);
    string getComputerName();

    ofxJSONElement json;
    string computerName;

    FILE *fp;
    int status;
    char path[1];
};

extern erSettings settings;