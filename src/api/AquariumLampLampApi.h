#pragma once
#include "AquariumLampBaseApi.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampLampApi final : public AquariumLampBaseApi{
public:
    static const char Scheduled[];
    static const char Manual[];

    using AquariumLampBaseApi::AquariumLampBaseApi;
    void begin() const override;

protected:
    static bool _setLampMode(const char *mode);                  // возвращает true если принято
};

