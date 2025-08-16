#pragma once
#include <app/MqttAppStateV2.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include "AppStateV1.h"
#include "AppStateV2.h"

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V3_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V3_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V3_SCHEDULES 12

namespace aquarium_lamp {
class AppStateV3 : public MqttAppStateV2<AQUARIUM_LAMP_STATE_V3_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x21781F30;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V3_CHANNELS, AQUARIUM_LAMP_STATE_V3_SCHEDULES> lamp;
    int x{0};
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
    void migrateFrom(aquarium_lamp::AppStateV1 *origState);
    void migrateFrom(aquarium_lamp::AppStateV2 *origState);
};
};