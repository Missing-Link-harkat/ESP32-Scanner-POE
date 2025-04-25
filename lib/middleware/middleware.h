#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include "Arduino.h"
#include "config.h"

class MiddlewareClass {
    public:
        MiddlewareClass();
        const char* getDeviceName();
        const char* getDeviceType();
        const char* getDeviceID();
        const char* getDeviceVersion();
        const char* getDeviceOwner();
        bool isDeveloperMode();
        void printSystemInfo();
    private:
        const char* _deviceName;
        const char* _deviceType;
        const char* _deviceID;
        const char* _deviceVersion;
        const char* _deviceOwner;
        bool _developerMode;
};

extern MiddlewareClass Middleware;

#endif