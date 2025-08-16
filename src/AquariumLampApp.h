#pragma once
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <sensor/temperature/Dallas/FilteredDallasSensor.h>
#include <sensor/temperature/TriggeredTemperatureSensor.h>
#include <switch/DigitalPinSwitch.h>
#include <switch/AutoSwitch.h>
#include <sensor/DebouncingSensor.h>
#include <hass/app/HaApplication.h>
#include "AquariumLampAppStateV6.h"
#include "Config.h"
#include <switch/SmoothPwmSwitch.h>
#include <hass/devices/scheduled_switch/HassScheduledLedArray.h>
#include "hass/devices/sensor/HassDallasSensor.h"
#include "hass/devices/HassAutoSwitch.h"
#include <schedule/ScheduledLedArray.h>
#include "api/AquariumLampDeviceApi.h"
#include "api/AquariumLampLampApi.h"
#include "api/AquariumLampWifiApi.h"
#include "api/AquariumLampNtpApi.h"

#include "device/OneWireDeviceScanner.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampApp : public HaApplication<AquariumLampAppStateV6>
{
public:
    SMART_STRING_DEF_CONST(LampUniqId);
    SMART_STRING_DEF_CONST(LampName);
    SMART_STRING_DEF_CONST(TempSensorUniqId);
    SMART_STRING_DEF_CONST(TempSensorName);
    SMART_STRING_DEF_CONST(FanSwitchUniqId);
    SMART_STRING_DEF_CONST(FanSwitchName);

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
    OneWire _oneWire;
    DallasTemperature _dallasSensors;
    FilteredDallasSensor _tempSensor;
    TriggeredTemperatureSensor _overheatingSensor;
    DigitalPinSwitch _fanSwitch;
    AutoSwitch _fanAutoSwitch;
    time_t _lastStateUpdatedAt;
    OneWireDeviceScanner _oneWireDeviceScanner;
    time_t _lastScannedAt;

    HassScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _haLamp;
    HassDallasSensor _haTempSensor;
    HassAutoSwitch _haFanSwitch;

    AquariumLampDeviceApi _deviceApi;
    AquariumLampLampApi _lampApi;
    AquariumLampWifiApi _wifiApi;
    AquariumLampNtpApi _ntpApi;

    void _updateSensorsStates();
    void _scanDevices();
};
