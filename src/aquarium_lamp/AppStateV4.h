#pragma once
#include <app/MqttAppStateV3.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include "AppStateV1.h"
#include "AppStateV2.h"
#include "AppStateV3.h"

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V4_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V4_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V4_SCHEDULES 12

namespace aquarium_lamp {
class AppStateV4 : public MqttAppStateV3<AQUARIUM_LAMP_STATE_V4_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x22781A39;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V4_CHANNELS, AQUARIUM_LAMP_STATE_V4_SCHEDULES> lamp;
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
    void migrateFrom(aquarium_lamp::AppStateV1 *origState);
    void migrateFrom(AppStateV2 *origState);
    void migrateFrom(AppStateV3 *origState);
};
};
