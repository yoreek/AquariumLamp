#include "App.h"
#include <switch/LedcPwmSwitch.h>

namespace aquarium_lamp {
extern AppStateV7 appState;

SMART_STRING_INIT_CONST(App, LampUniqId, "lamp2");
SMART_STRING_INIT_CONST(App, LampName, "Lamp2");
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
      _dallasSensors(&_oneWire),
      _tempSensor(&_dallasSensors, &appState.tempSensor),
      _overheatingSensor(&_tempSensor, &appState.overheatingSensor),
      _fanSwitch(Config::FanPin),
      _fanAutoSwitch(&_fanSwitch, &appState.fanSwitch),

      _lastStateUpdatedAt(0),
      _oneWireDeviceScanner(&_oneWire),
      _lastScannedAt(0),

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
      _mqttApi(_webServer)
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
}

void App::_processLedSwitches()
{
    for (auto & led : _ledArray)
        led->loop();
}

void App::loop(uint32_t uptime)
{
    HaApplication::loop(uptime);
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
    // _fanSwitch.toggle(!_fanSwitch.isTurnedOn());
    _scanDevices();
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

void App::_scanDevices()
{
    if (_oneWireDeviceScanner.inProgress()) {
        LOG_DEBUG("INPROGRESS");
        return;
    }

    if (_oneWireDeviceScanner.completed()) {
        LOG_DEBUG("Found: %d devices", _oneWireDeviceScanner.devicesCount());
        for (size_t i = 0; i < _oneWireDeviceScanner.devicesCount(); i++) {
            const auto &device = _oneWireDeviceScanner.devices()[i];
            LOG_DEBUG("Device %d: %02X %02X %02X %02X %02X %02X %02X %02X",
                      i,
                      device[0], device[1], device[2], device[3],
                      device[4], device[5], device[6], device[7]);
        }
        _oneWireDeviceScanner.reset();
        _lastScannedAt = millis();
    }

    if ((millis() - _lastScannedAt) >= 10000) {
        LOG_DEBUG("start scanning for OneWire devices");
        _oneWireDeviceScanner.start();
    }
}
}
