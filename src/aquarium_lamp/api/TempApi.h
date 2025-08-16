#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class TempApi final : public BaseApi {
public:
    using BaseApi::BaseApi;
    void begin() const override;
    static void addressToString(const byte* address, char *buf);
    static bool parseAddress(const char *addr, byte *arr);
};
};
