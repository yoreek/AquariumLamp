#pragma once
#include <app/MqttAppStateV4.h>
#include <schedule/ScheduledLedArrayStateV1.h>

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V6_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V6_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V6_SCHEDULES 10

class AquariumLampAppStateV6 : public MqttAppStateV4<AQUARIUM_LAMP_STATE_V6_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x78680335;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V6_CHANNELS, AQUARIUM_LAMP_STATE_V6_SCHEDULES> lamp;
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
};
