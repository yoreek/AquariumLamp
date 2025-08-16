#pragma once
#include <ArduinoJson.h>
#include "BaseApi.h"


REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class DeviceApi final : public BaseApi {
public:
    using BaseApi::BaseApi;
    void begin() const override;
};
};
