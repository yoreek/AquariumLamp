#include "AppStateV5.h"
#include <debug/Debug.h>

namespace aquarium_lamp {
bool AppStateV5::isUpdated() const
{
    return MqttAppStateV3::isUpdated() || lamp.isUpdated();
}

void AppStateV5::markAsSaved()
{
    MqttAppStateV3::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV5::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == aquarium_lamp::AppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        aquarium_lamp::AppStateV1 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV1), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        aquarium_lamp::AppStateV2 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV2), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        aquarium_lamp::AppStateV3 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV3), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV4::magicKey) {
        LOG_INFO("migrate from V4");
        aquarium_lamp::AppStateV4 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV4), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
}
}
