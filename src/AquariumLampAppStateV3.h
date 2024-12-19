#pragma once
#include <app/MqttAppStateV2.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include "AquariumLampAppStateV1.h"
#include "AquariumLampAppStateV2.h"

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V3_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V3_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V3_SCHEDULES 12

class AquariumLampAppStateV3 : public MqttAppStateV2<AQUARIUM_LAMP_STATE_V3_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x21781F30;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V3_CHANNELS, AQUARIUM_LAMP_STATE_V3_SCHEDULES> lamp;
    int x{0};
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
    void migrateFrom(AquariumLampAppStateV1 *origState);
    void migrateFrom(AquariumLampAppStateV2 *origState);
};
