#pragma once
#include <app/MqttAppStateV1.h>
#include <schedule/ScheduledLedArrayStateV1.h>

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V1_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V1_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V1_SCHEDULES 10

class AquariumLampAppStateV1 : public MqttAppStateV1<AQUARIUM_LAMP_STATE_V1_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x095475A4;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V1_CHANNELS, AQUARIUM_LAMP_STATE_V1_SCHEDULES> lamp;
    bool isUpdated() const;
    void markAsSaved();
};
