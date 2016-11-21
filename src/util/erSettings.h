#pragma once
#include "ofMain.h"
#include "erLogger.h"
#include "ofxJSON.h"

#define ER_SETTINGS_FILE "settings.json"

#define ER_FULLSCREEN_BY_DEFAULT "fullscreen-by-default"
#define ER_LOG_TO_FILE_ENABLED "enable-log-to-file"
#define ER_TEST_CONTENT_MODE "test-content-mode"
#define ER_NUMBER_OF_VIDEO_CHANNELS "number-of-video-channels"
#define ER_MIN_DECOY_DURATION "min-decoy-duration-secs"

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
#define ER_HIGHEST_BPM "highest-bpm"
#define ER_LOWEST_BPM "lowest-bpm"
#define ER_BPM_PERIOD_SECS "bpm-period-secs"
#define ER_BEGIN_AT "begin-at"
#define ER_END_AT "end-at"

#define ER_MEDIA_SEQUENCING "media-sequencing"
#define ER_SHORTEST_DELAY "shortest-delay"
#define ER_LONGEST_DELAY "longest-delay"

#define ER_MEDIA_FILES "media-files"

#define ER_LIVE_PATH "live-path"
#define ER_PREVIEW_PATH "preview-path"
#define ER_TEST_CONTENT_LIVE_PATH "test-content-live-path"
#define ER_TEST_CONTENT_PREVIEW_PATH "test-content-preview-path"
#define ER_BREATHING_SOUND_PATH "breathing-sound-path"

#define ER_LIVE_PATH_SYNCED "live-path-synced"
#define ER_PREVIEW_PATH_SYNCED "preview-path-synced"
#define ER_TEST_CONTENT_LIVE_PATH_SYNCED "test-content-live-path-synced"
#define ER_TEST_CONTENT_PREVIEW_PATH_SYNCED "test-content-preview-path-synced"
#define ER_BREATHING_SOUND_PATH_SYNCED "breathing-sound-path-synced"

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

    bool fullscreenByDefault;
    bool logToFileEnabled;
    bool testContentMode;
    int numChannels;
    float minDecoyDuration;

    bool isServer;
    bool isEcg;

    float masterVolume;
    float breathingVolume;
    float videoVolume;
    float ecgVolume;
    float staticVolume;

    float ecgHighestBpm;
    float ecgLowestBpm;
    int ecgBpmPeriodSecs;
    float ecgBeginBeepAt;
    float ecgEndBeepAt;

    int shortestSequenceDelay;
    int longestSequenceDelay;

    string liveMediaPath;
    string previewMediaPath;
    string breathingSoundPath;
    string liveMediaDir;
    string previewMediaDir;
    string textMediaDir;
    string videoFileExtension;
    string textFileExtension;

    bool serverDrawingEnabled;
    bool clientDrawingEnabled;
    string computerName;

protected:
    void initSettings();
    void applySettings(ofxJSONElement& json);
    string getComputerName();

    ofxJSONElement json;

    FILE *fp;
    int status;
    char path[1];
};

extern erSettings settings;
