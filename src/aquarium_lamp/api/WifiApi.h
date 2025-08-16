#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class WifiApi final : public BaseApi {
public:
    using BaseApi::BaseApi;
    void begin() const override;

protected:

    static bool _updateStaSettings(const JsonArray &arr) ;
    static void _updateApSettings(const JsonObject &node) ;
};
};
