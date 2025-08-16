#pragma once
#include <app/MqttAppStateV1.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include "AppStateV1.h"

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V2_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V2_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V2_SCHEDULES 12

namespace aquarium_lamp {
class AppStateV2 : public MqttAppStateV1<AQUARIUM_LAMP_STATE_V2_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x11781F3B;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V2_CHANNELS, AQUARIUM_LAMP_STATE_V2_SCHEDULES> lamp;
    int x{0};
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
    void migrateFrom(aquarium_lamp::AppStateV1 *appStateV1);
};
};
