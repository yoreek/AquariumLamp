#include "AppStateV3.h"
#include <debug/Debug.h>

namespace aquarium_lamp {
bool AppStateV3::isUpdated() const
{
    return MqttAppStateV2::isUpdated() || lamp.isUpdated();
}

void AppStateV3::markAsSaved()
{
    MqttAppStateV2::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV3::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == aquarium_lamp::AppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        aquarium_lamp::AppStateV1 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV1), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        aquarium_lamp::AppStateV2 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV2), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AppStateV3::migrateFrom(aquarium_lamp::AppStateV1 *origState)
{
    MqttAppStateV2::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AppStateV3::migrateFrom(aquarium_lamp::AppStateV2 *origState)
{
    MqttAppStateV2::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
}
