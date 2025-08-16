#include "AppStateV6.h"
#include <debug/Debug.h>
#include "AppStateV1.h"
#include "AppStateV2.h"
#include "AppStateV3.h"
#include "AppStateV4.h"
#include "AppStateV5.h"

namespace aquarium_lamp {
bool AppStateV6::isUpdated() const
{
    return MqttAppStateV4::isUpdated() || lamp.isUpdated();
}

void AppStateV6::markAsSaved()
{
    MqttAppStateV4::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV6::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == aquarium_lamp::AppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        aquarium_lamp::AppStateV1 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV1), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        aquarium_lamp::AppStateV2 origState;
        storage->read(&origState, sizeof(AppStateV2), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        AppStateV3 origState;
        storage->read(&origState, sizeof(AppStateV3), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV4::magicKey) {
        LOG_INFO("migrate from V4");
        AppStateV4 origState;
        storage->read(&origState, sizeof(AppStateV4), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV5::magicKey) {
        LOG_INFO("migrate from V5");
        AppStateV5 origState;
        storage->read(&origState, sizeof(AppStateV5), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
}
}
