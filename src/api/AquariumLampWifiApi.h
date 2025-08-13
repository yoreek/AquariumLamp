#pragma once
#include "AquariumLampBaseApi.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampWifiApi final : public AquariumLampBaseApi {
public:
    using AquariumLampBaseApi::AquariumLampBaseApi;
    void begin() const override;

protected:

    static bool _updateStaSettings(const JsonArray &arr) ;
    static void _updateApSettings(const JsonObject &node) ;
};
