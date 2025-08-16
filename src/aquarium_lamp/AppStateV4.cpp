#include "AppStateV4.h"
#include <debug/Debug.h>

namespace aquarium_lamp {
bool AppStateV4::isUpdated() const
{
    return MqttAppStateV3::isUpdated() || lamp.isUpdated();
}

void AppStateV4::markAsSaved()
{
    MqttAppStateV3::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV4::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
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
    else if (prevMagicKey == aquarium_lamp::AppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        aquarium_lamp::AppStateV3 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV3), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AppStateV4::migrateFrom(aquarium_lamp::AppStateV1 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AppStateV4::migrateFrom(aquarium_lamp::AppStateV2 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AppStateV4::migrateFrom(aquarium_lamp::AppStateV3 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
}
