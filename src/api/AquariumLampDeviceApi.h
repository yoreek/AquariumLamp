#pragma once
#include "net/WebServerManager.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampDeviceApi {
public:
    explicit AquariumLampDeviceApi(WebServerManager& webServer);
    void begin() const;

protected:
    WebServerManager& _webServer;
};
