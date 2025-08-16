#pragma once
#include "BaseApi.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class LampApi final : public BaseApi{
public:
    static const char Scheduled[];
    static const char Manual[];

    using BaseApi::BaseApi;
    void begin() const override;

protected:
    static bool _setLampMode(const char *mode);                  // возвращает true если принято
};
}
