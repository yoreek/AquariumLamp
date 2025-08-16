#pragma once
#include <app/MqttAppStateV4.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include <sensor/temperature/Dallas/FilteredDallasSensorStateV1.h>
#include <sensor/temperature/TriggeredTemperatureSensorStateV1.h>
#include <switch/AutoSwitchStateV1.h>

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V6_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V6_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V6_SCHEDULES 10

class AquariumLampAppStateV6 : public MqttAppStateV4<AQUARIUM_LAMP_STATE_V6_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x78680339;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V6_CHANNELS, AQUARIUM_LAMP_STATE_V6_SCHEDULES> lamp;
    FilteredDallasSensorStateV1 tempSensor;
    TriggeredTemperatureSensorStateV1 overheatingSensor;
    AutoSwitchStateV1 fanSwitch;

    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
};
