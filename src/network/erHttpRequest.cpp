#include "erHttpRequest.h"

void erHttpRequest::send(string _url){
    url = _url;
    startThread(true);
}
