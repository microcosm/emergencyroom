#pragma once
#include "ofMain.h"
#include "erLogger.h"

#define ER_TIMEOUT_MS 600000 //10 minutes

class erKillTimer{

public:
    void setup();
    void update();
    void registerMessageRecieved();

protected:
    uint64_t lastMessageRecievedTime;
};
