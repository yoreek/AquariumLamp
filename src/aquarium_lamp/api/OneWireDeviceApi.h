#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class OneWireDeviceApi final : public BaseApi {
public:
    using BaseApi::BaseApi;
    void begin() const override;
};
};
