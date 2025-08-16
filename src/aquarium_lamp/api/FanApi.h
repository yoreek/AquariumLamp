#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class FanApi final : public BaseApi {
public:
    static const char AutoMode[];
    static const char OffMode[];
    static const char OnMode[];
    static const char PauseMode[];

    using BaseApi::BaseApi;
    void begin() const override;

protected:
    static bool _setFanMode(const char *mode);
};
};
