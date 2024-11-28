#pragma once
#include <hass/app/HaApplication.h>
#include "AquariumLampConfig.h"
#include "Config.h"
#include <switch/SmoothPwmSwitch.h>
#include <hass/devices/scheduled_switch/HassScheduledLedArray.h>
#include <schedule/ScheduledLedArray.h>

REEFDUINO_NAMESPACE_USING

class AquariumLampApp : public HaApplication
{
public:
    SMART_STRING_DEF_CONST(LampUniqId);
    SMART_STRING_DEF_CONST(LampName);

    AquariumLampApp();
    void begin() override;
    void loop() override;

protected:
    SmoothPwmSwitch *_ledArray[LEDS_NUM];
    void _processLedSwitches();
    ScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _lamp;
    HassScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _haLamp;
};
