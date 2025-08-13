#pragma once
#include <hass/app/HaApplication.h>
#include "AquariumLampAppStateV6.h"
#include "Config.h"
#include <switch/SmoothPwmSwitch.h>
#include <hass/devices/scheduled_switch/HassScheduledLedArray.h>
#include <schedule/ScheduledLedArray.h>
#include "api/AquariumLampDeviceApi.h"
#include "api/AquariumLampLampApi.h"
#include "api/AquariumLampWifiApi.h"
#include "api/AquariumLampNtpApi.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampApp : public HaApplication<AquariumLampAppStateV6>
{
public:
    SMART_STRING_DEF_CONST(LampUniqId);
    SMART_STRING_DEF_CONST(LampName);

    AquariumLampApp();
    void begin() override;
    void loop() override { HaApplication<AquariumLampAppStateV6>::loop(); };
    void loop(uint32_t uptime) override;
    void loop1s(uint32_t uptime) override;
    void loop200ms(uint32_t uptime) override;
    [[nodiscard]] float getBrightness(const uint8_t channel) const {
        if (channel >= LEDS_NUM || channel < 0) {
            return 0.0f; // Invalid channel
        }
        return _ledArray[channel]->read();
    };
    bool setManualBrightness(const uint8_t channel, const float brightness)
    {
        return _lamp.setManualBrightness(channel, brightness);
    };
    [[nodiscard]] float getManualBrightness(const uint8_t channel) const
    {
        return _lamp.getManualBrightness(channel);
    };

protected:
    SmoothPwmSwitch *_ledArray[LEDS_NUM];
    void _processLedSwitches();
    ScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _lamp;
    HassScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _haLamp;
    AquariumLampDeviceApi _deviceApi;
    AquariumLampLampApi _lampApi;
    AquariumLampWifiApi _wifiApi;
    AquariumLampNtpApi _ntpApi;
};
