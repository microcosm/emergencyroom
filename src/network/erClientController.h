#pragma once
#include "erSettings.h"
#include "erNetwork.h"
#include "erUtils.h"

#define ER_FRAMECOUNTER_MAX 60

class erClientController{
    
public:
    void setup(erNetwork* _network);
    void launchClients();
    
protected:
    int frameCounter;
    erNetwork* network;
};
