#include "erClientController.h"

void erClientController::launchClients(){
    for(int i = 0; i < settings.clientIPs.size(); i++){
        cout << settings.clientIPs.at(i) << endl;
    }
}
