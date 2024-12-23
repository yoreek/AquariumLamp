#pragma once
#include <Arduino.h>
#include <string/SmartString.h>

REEFDUINO_NAMESPACE_USING

class Config
{
public:
    SMART_STRING_DEF_CONST(DeviceSoftwareVersion);
    SMART_STRING_DEF_CONST(DeviceUniqId);
    SMART_STRING_DEF_CONST(DeviceName);
#if defined(ESP8266)
    static const char *MqttFingerprint;
#else
    static const char *MqttCaCert;
#endif
    static const uint8_t LedPin[LEDS_NUM];
    static const uint8_t LedFirstChannel;
    static const uint32_t LedFreq;
    static const uint8_t LedResolution;
    static const bool LedInverted;
    static const float LedMaxChangeAtOnce;
};
