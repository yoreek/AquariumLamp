#pragma once
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <sensor/temperature/Dallas/FilteredDallasSensor.h>
#include <sensor/temperature/TriggeredTemperatureSensor.h>
#include <switch/DigitalPinSwitch.h>
#include <switch/AutoSwitch.h>
#include <hass/app/HaApplication.h>
#include "AppStateV7.h"
#include "../Config.h"
#include <switch/SmoothPwmSwitch.h>
#include <hass/devices/scheduled_switch/HassScheduledLedArray.h>
#include "hass/devices/sensor/HassDallasSensor.h"
#include "hass/devices/HassAutoSwitch.h"
#include <schedule/ScheduledLedArray.h>
#include <rtc/DS3231.h>
#include <rtc/RtcManager.h>
#include "api/DeviceApi.h"
#include "api/LampApi.h"
#include "api/WifiApi.h"
#include "api/NtpApi.h"
#include "api/TempApi.h"
#include "api/FanApi.h"
#include "api/MqttApi.h"
#include "api/OneWireDeviceApi.h"

#include "device/OneWireDeviceScanner.h"

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
class App : public HaApplication<aquarium_lamp::AppStateV7>
{
public:
    SMART_STRING_DEF_CONST(LampUniqId);
    SMART_STRING_DEF_CONST(LampName);
    SMART_STRING_DEF_CONST(TempSensorUniqId);
    SMART_STRING_DEF_CONST(TempSensorName);
    SMART_STRING_DEF_CONST(FanSwitchUniqId);
    SMART_STRING_DEF_CONST(FanSwitchName);

    App();
    void begin() override;
    void loop() override { HaApplication<aquarium_lamp::AppStateV7>::loop(); };
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
    FilteredDallasSensor &getTempSensor() { return _tempSensor; };
    OneWireDeviceScanner &getOneWireDeviceScanner() { return _oneWireDeviceScanner; };
    inline RtcManager *rtc() { return &_rtc; };
    static void syncTime(time_t now, NtpManager *ntpManager, void *data);

protected:
    SmoothPwmSwitch *_ledArray[LEDS_NUM];
    void _processLedSwitches();
    ScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _lamp;
    OneWire _oneWire;
    TwoWire _i2cOne;
    DS3231 _ds3231;
    RtcManager _rtc;
    DallasTemperature _dallasSensors;
    FilteredDallasSensor _tempSensor;
    TriggeredTemperatureSensor _overheatingSensor;
    DigitalPinSwitch _fanSwitch;
    AutoSwitch _fanAutoSwitch;
    time_t _lastStateUpdatedAt;
    OneWireDeviceScanner _oneWireDeviceScanner;

    HassScheduledLedArray<LEDS_NUM, LEDS_SCHEDULES> _haLamp;
    HassDallasSensor _haTempSensor;
    HassAutoSwitch _haFanSwitch;

    DeviceApi _deviceApi;
    LampApi _lampApi;
    WifiApi _wifiApi;
    NtpApi _ntpApi;
    TempApi _tempApi;
    FanApi _fanApi;
    MqttApi _mqttApi;
    OneWireDeviceApi _oneWireDeviceApi;

    void _updateSensorsStates();
};
};
