#pragma once
#include <app/MqttAppStateV3.h>
#include <schedule/ScheduledLedArrayStateV1.h>
#include "AppStateV1.h"
#include "AppStateV2.h"
#include "AppStateV3.h"
#include "AppStateV4.h"

REEFDUINO_NAMESPACE_USING

#define AQUARIUM_LAMP_STATE_V5_AP_COUNT 2
#define AQUARIUM_LAMP_STATE_V5_CHANNELS 5
#define AQUARIUM_LAMP_STATE_V5_SCHEDULES 10

namespace aquarium_lamp {
class AppStateV5 : public MqttAppStateV3<AQUARIUM_LAMP_STATE_V5_AP_COUNT>
{
public:
    static const uint32_t magicKey = 0x18781A81;
    ScheduledLedArrayStateV1<AQUARIUM_LAMP_STATE_V5_CHANNELS, AQUARIUM_LAMP_STATE_V5_SCHEDULES> lamp;
    bool isUpdated() const;
    void markAsSaved();
    void migrate(AbstractStorage *storage, uint32_t prevMagicKey);
};
};
