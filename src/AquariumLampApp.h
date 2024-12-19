#pragma once
#include <hass/app/HaApplication.h>
#include "AquariumLampAppStateV5.h"
#include "Config.h"
#include <switch/SmoothPwmSwitch.h>
#include <hass/devices/scheduled_switch/HassScheduledLedArray.h>
#include <schedule/ScheduledLedArray.h>

REEFDUINO_NAMESPACE_USING

class AquariumLampApp : public HaApplication<AquariumLampAppStateV5>
{
public:
    SMART_STRING_DEF_CONST(LampUniqId);
    SMART_STRING_DEF_CONST(LampName);

    AquariumLampApp();
    void begin() override;
    void loop() override { HaApplication<AquariumLampAppStateV5>::loop(); };
    void loop(uint32_t uptime) override;
    void loop1s(uint32_t uptime) override;
    void loop200ms(uint32_t uptime) override;

protected:
    SmoothPwmSwitch *_ledArray[LEDS_NUM];
    void _processLedSwitches();
    ScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _lamp;
    HassScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _haLamp;
};
