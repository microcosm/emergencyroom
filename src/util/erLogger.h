#pragma once
#include "ofMain.h"

extern string erProgramId;
extern bool erFileLoggingEnabled;

void erEnableFileLogging(bool isServer);
void erDisableFileLogging();
void erLog(string methodSignature, string message);