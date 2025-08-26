#include "App.h"
#include <switch/LedcPwmSwitch.h>

namespace aquarium_lamp {
extern AppStateV7 appState;

SMART_STRING_INIT_CONST(App, LampUniqId, "lamp");
SMART_STRING_INIT_CONST(App, LampName, "Lamp");
SMART_STRING_INIT_CONST(App, TempSensorUniqId, "temp");
SMART_STRING_INIT_CONST(App, TempSensorName, "Temp");
SMART_STRING_INIT_CONST(App, FanSwitchUniqId, "fan");
SMART_STRING_INIT_CONST(App, FanSwitchName, "Fan");

App::App()
    : HaApplication(Config::DeviceUniqId,
                    Config::DeviceName,
                    &appState),
      _ledArray(),
      _lamp(reinterpret_cast<AbstractPwmSwitch **>(_ledArray), &appState.lamp),
      _oneWire(Config::OneWirePin),
#if defined(ESP32)
      _i2cOne(0),
#elif defined(ESP8266)
      _i2cOne(),
#endif
      _ds3231(&_i2cOne),
      _rtc(&_ds3231),
      _dallasSensors(&_oneWire),
      _tempSensor(&_dallasSensors, &appState.tempSensor),
      _overheatingSensor(&_tempSensor, &appState.overheatingSensor),
      _fanSwitch(Config::FanPin),
      _fanAutoSwitch(&_fanSwitch, &appState.fanSwitch),

      _lastStateUpdatedAt(0),
      _oneWireDeviceScanner(&_oneWire),

      _haLamp(LampUniqId,
              &_lamp,
              &appState.lamp,
              reinterpret_cast<AbstractPwmSwitch **>(_ledArray)),
      _haTempSensor(TempSensorUniqId, &appState.tempSensor),
      _haFanSwitch(FanSwitchUniqId, &_fanAutoSwitch, &appState.fanSwitch),
      _lampApi(_webServer),
      _deviceApi(_webServer),
      _wifiApi(_webServer),
      _ntpApi(_webServer),
      _tempApi(_webServer),
      _fanApi(_webServer),
      _mqttApi(_webServer),
      _oneWireDeviceApi(_webServer)
{
    _fanAutoSwitch.addDependency(&_overheatingSensor, true);
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

    _haTempSensor.setName(&TempSensorName);
    _mainDevice.addDevice(&_haTempSensor);

    _haFanSwitch.setName(&FanSwitchName);
    _mainDevice.addDevice(&_haFanSwitch);
}

void App::begin()
{
    HaApplication::begin();
    WiFi.setSleep(false);
#if defined(ESP8266)
    setMqttFingerprint(Config::MqttFingerprint);
#else
    setMqttCACert(Config::MqttCaCert);
#endif

    _ntp.onSync(syncTime, this);
    _i2cOne.begin(Config::I2cOneSda, Config::I2cOneScl);
    _i2cOne.setClock(Config::I2cOneFreq);

    for (auto & led : _ledArray)
    {
        led->begin();
    }
    _fanSwitch.begin();

    _lampApi.begin();
    _deviceApi.begin();
    _wifiApi.begin();
    _ntpApi.begin();
    _tempApi.begin();
    _fanApi.begin();
    _mqttApi.begin();
    _oneWireDeviceApi.begin();
}

void App::_processLedSwitches()
{
    for (auto & led : _ledArray)
        led->loop();
}

void App::loop(const uint32_t uptime)
{
    HaApplication::loop(uptime);
    _rtc.loop(uptime);
    _lamp.loop(uptime);
    _tempSensor.loop(uptime);
    _overheatingSensor.loop(uptime);
    _fanAutoSwitch.loop(uptime);
    _oneWireDeviceScanner.loop(uptime);
}

void App::loop1s(uint32_t uptime)
{
    HaApplication::loop1s(uptime);
    _haLamp.loop1s(uptime);
    _updateSensorsStates();
}

void App::loop200ms(uint32_t uptime)
{
    HaApplication::loop200ms(uptime);
    _processLedSwitches();
}

void App::_updateSensorsStates()
{
    if ((millis() - _lastStateUpdatedAt) >= Config::SensorStateUpdateInterval) {
        if (_tempSensor.isReady()) {
            _haTempSensor.setState(_tempSensor.getValue());
        }
        _lastStateUpdatedAt = millis();
    }
}

void App::syncTime(const time_t now, NtpManager *ntpManager, void *data)
{
    auto *app = static_cast<App *> (data);
    const rd::DateTime x(now);
    app->rtc()->adjustTime(x);

    WifiApplication::syncTime(now, ntpManager, data);
}

}
