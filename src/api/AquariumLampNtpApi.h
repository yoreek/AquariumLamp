#pragma once
#include "AquariumLampBaseApi.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampNtpApi final : public AquariumLampBaseApi {
public:
    using AquariumLampBaseApi::AquariumLampBaseApi;
    void begin() const override;
};
