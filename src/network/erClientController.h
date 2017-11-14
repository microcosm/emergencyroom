#pragma once
#include "erSettings.h"
#include "erNetwork.h"
#include "erUtils.h"
#include "erHttpRequest.h"
#include "erSettings.h"

#define ER_FRAMECOUNTER_MAX 2400 //~40 secs

class erClientController{
    
public:
    void setup(erNetwork* _network);
    void openClientApps();
    void closeClientApps();
    
protected:
    void request(string url);
    void cleanRequests();
    int frameCounter;
    erNetwork* network;
    string url;
    vector<erHttpRequest*> httpRequests;
};
