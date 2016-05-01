#pragma once
#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioUnitManager.h"
#include "erLogger.h"
#include "ofxJSON.h"

#define ER_SETTINGS_FILE "settings.json"

#define ER_FULLSCREEN_BY_DEFAULT "fullscreen-by-default"
#define ER_LOG_TO_FILE_ENABLED "enable-log-to-file"
#define ER_NUMBER_OF_VIDEO_CHANNELS "number-of-video-channels"

#define ER_MACHINE_NAMES "machine-names"
#define ER_SERVER "server"
#define ER_ECG "ecg"

#define ER_SERVER_VOLUMES "server-volumes"
#define ER_MASTER "master"
#define ER_VIDEO "video"
#define ER_ECG "ecg"
#define ER_STATIC "static"

#define ER_ECG_BEEP "ecg-beep"
#define ER_BPM "bpm"
#define ER_BEGIN_AT "begin-at"
#define ER_END_AT "end-at"

#define ER_MEDIA_FILES "media-files"
#define ER_LIVE_PATH "live-path"
#define ER_PREVIEW_PATH "preview-path"
#define ER_TEST_SOUND "test-sound"
#define ER_TEST_VIDEO "test-video"
#define ER_LIVE_DIR "live-dir"
#define ER_PREVIEW_DIR "preview-dir"
#define ER_ALLOWED_VIDEO_EXTENSIONS "allowed-video-extensions"

class erSettings{

public:
    void load();
    void toggleServerDrawing();
    void toggleClientDrawing();

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

    string liveMediaPath;
    string previewMediaPath;
    string testSoundPath;
    string testVideoPath;
    string liveMediaDir;
    string previewMediaDir;
    string allowedVideoExtensions;

    bool serverDrawingEnabled;
    bool clientDrawingEnabled;

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