#pragma once
#include "ofMain.h"

extern string erProgramId;
extern bool erFileLoggingEnabled;

bool isFileLogging();
void enableFileLogging(bool isServer);
void disableFileLogging();
void erLog(string methodSignature, string message);