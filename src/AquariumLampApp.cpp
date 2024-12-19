#include "AquariumLampApp.h"
#include <switch/LedcPwmSwitch.h>

extern AquariumLampAppStateV5 appState;

SMART_STRING_INIT_CONST(AquariumLampApp, LampUniqId, "lamp");
SMART_STRING_INIT_CONST(AquariumLampApp, LampName, "Lamp");

AquariumLampApp::AquariumLampApp()
        : HaApplication(Config::DeviceUniqId,
                        Config::DeviceName,
                        &appState),
          _ledArray(),
          _lamp((AbstractPwmSwitch **) _ledArray, &appState.lamp),
          _haLamp(LampUniqId,
                  &_lamp,
                  &appState.lamp,
                  (AbstractPwmSwitch **) _ledArray)
{
    _haLamp.setName(&LampName);
    for (uint8_t i = 0; i < LEDS_NUM; i++) {
        _ledArray[i] = new SmoothPwmSwitch(
                new LedcPwmSwitch(
                        Config::LedPin[i],
                        Config::LedFirstChannel + i,
                        Config::LedFreq,
                        Config::LedResolution,
                        Config::LedInverted),
                Config::LedMaxChangeAtOnce);
    }
    _mainDevice.addDevice(&_haLamp);
}

void AquariumLampApp::begin()
{
    HaApplication::begin();
    WiFi.setSleep(false);
#if defined(ESP8266)
    setMqttFingerprint(Config::MqttFingerprint);
#else
    setMqttCACert(Config::MqttCaCert);
#endif
    for (auto & led : _ledArray)
    {
        led->begin();
    }
}

void AquariumLampApp::_processLedSwitches()
{
    for (auto & led : _ledArray)
        led->loop();
}

void AquariumLampApp::loop(uint32_t uptime)
{
    HaApplication::loop(uptime);
    _lamp.loop(uptime);
}

void AquariumLampApp::loop1s(uint32_t uptime)
{
    HaApplication::loop1s(uptime);
    _haLamp.loop1s(uptime);
}

void AquariumLampApp::loop200ms(uint32_t uptime)
{
    HaApplication::loop200ms(uptime);
    _processLedSwitches();
}
