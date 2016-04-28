#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erLogger.h"
#include "ofxJSON.h"

#define SETTINGS_FILE "settings.json"
#define FULLSCREEN_BY_DEFAULT "fullscreen-by-default"
#define SERVER_MASTER_VOLUME "server-master-volume"
#define SERVER_MACHINE_NAME "server-machine-name"
#define ECG_MACHINE_NAME "ecg-machine-name"
#define ECG_BPM "ecg-bpm"

class erSettings{

public:
    void load();
    float masterVolume;
    bool fullscreenByDefault;
    bool isServer;
    bool isEcg;
    float ecgPeriod;
    float ecgBpm;

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