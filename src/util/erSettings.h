#pragma once
#include "ofMain.h"
#include "erLogger.h"
#include "ofxJSON.h"

#define ER_SETTINGS_FILE "settings.json"

#define ER_OF_PORT "of-port"
#define ER_MANAGE_PORT "manage-port"

#define ER_SERVER_IP "server-ip"
#define ER_CLIENT_IPS "client-ips"

#define ER_FULLSCREEN_SERVER "fullscreen-server"
#define ER_FULLSCREEN_CLIENT "fullscreen-client"
#define ER_INITIAL_VIEW_MODE "initial-view-mode"
#define ER_FONT_PATH "font-path"

#define ER_LOG_TO_FILE_ENABLED "enable-log-to-file"
#define ER_TEST_CONTENT_MODE "test-content-mode"
#define ER_NUMBER_OF_VIDEO_CHANNELS "number-of-video-channels"
#define ER_MIN_DECOY_DURATION "min-decoy-duration-secs"

#define ER_MACHINE_NAMES "machine-names"
#define ER_SERVER "server"

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

#define ER_LIVE_DIR "live-dir"
#define ER_PREVIEW_DIR "preview-dir"
#define ER_VIDEO_FILE_EXTENSION "video-file-extension"

enum erViewMode {
    channels,
    ecg,
    audio
};

class erSettings{

public:
    void load();
    void toggleClientDrawing();
    void incrementViewMode();

    bool renderChannels();
    bool renderEcg();
    bool renderAudioUI();
    bool renderStatus();

    int ofPort;
    int managePort;

    string serverIP;
    vector<string> clientIPs;

    bool fullscreenServer;
    bool fullscreenClient;
    erViewMode viewMode;
    string fontPath;

    bool logToFileEnabled;
    bool testContentMode;
    int numChannels;
    float minDecoyDuration;

    bool isServer;
    bool isClient;

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
    string videoFileExtension;

    bool clientDrawingEnabled;
    string computerName;

protected:
    void initSettings();
    void applySettings(ofxJSONElement& json);
    string getComputerName();
    erViewMode getViewMode();

    ofxJSONElement json;

    FILE *fp;
    int status;
    char path[1];
};

extern erSettings settings;
