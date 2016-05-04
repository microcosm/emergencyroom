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
#define ER_BREATHING "breathing"
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
#define ER_TEST_SOUND_PATH "test-sound-path"
#define ER_TEST_VIDEO_PATH "test-video-path"
#define ER_BREATHING_SOUND_PATH "breathing-sound-path"
#define ER_LIVE_DIR "live-dir"
#define ER_PREVIEW_DIR "preview-dir"
#define ER_TEXT_DIR "text-dir"
#define ER_VIDEO_FILE_EXTENSION "video-file-extension"
#define ER_TEXT_FILE_EXTENSION "text-file-extension"

class erSettings{

public:
    void load();
    void toggleServerDrawing();
    void toggleClientDrawing();

    float masterVolume;
    float breathingVolume;
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
    string breathingSoundPath;
    string liveMediaDir;
    string previewMediaDir;
    string textMediaDir;
    string videoFileExtension;
    string textFileExtension;

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