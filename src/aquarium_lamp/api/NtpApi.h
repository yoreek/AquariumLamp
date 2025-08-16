#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class NtpApi final : public BaseApi {
public:
    using BaseApi::BaseApi;
    void begin() const override;
};
};
