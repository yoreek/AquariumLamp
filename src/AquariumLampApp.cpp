#include "AquariumLampApp.h"
#include "AquariumLampConfig.h"
#include <switch/LedcPwmSwitch.h>

extern AquariumLampConfig appConfig;

SMART_STRING_INIT_CONST(AquariumLampApp, LampUniqId, "lamp");
SMART_STRING_INIT_CONST(AquariumLampApp, LampName, "Lamp");

AquariumLampApp::AquariumLampApp()
        : HaApplication(Config::DeviceUniqId,
                        Config::DeviceName,
                        &appConfig),
          _ledArray(),
          _lamp((AbstractPwmSwitch **) _ledArray, &appConfig.lamp),
          _haLamp(LampUniqId,
                  &_lamp,
                  &appConfig.lamp,
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
                Config::LedMaxChangeAtOnce,
                Config::LedUpdateDelay);
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

void AquariumLampApp::loop()
{
    HaApplication::loop();
    _config->loop();
    _lamp.loop();
    _haLamp.loop();
    _processLedSwitches();
}
