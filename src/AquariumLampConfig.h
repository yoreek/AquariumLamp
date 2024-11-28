#pragma once
#include <app/MqttAppConfig.h>
#include <schedule/ScheduledLedArrayConfig.h>

REEFDUINO_NAMESPACE_USING

class AquariumLampConfig : public MqttAppConfig
{
public:
     ScheduledLedArrayConfig<LEDS_NUM, LEDS_SCHEDULES> lamp;
};
