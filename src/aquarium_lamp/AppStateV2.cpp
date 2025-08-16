#include "AppStateV2.h"

namespace aquarium_lamp {
bool AppStateV2::isUpdated() const
{
    return MqttAppStateV1::isUpdated() || lamp.isUpdated();
}

void AppStateV2::markAsSaved()
{
    MqttAppStateV1::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV2::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == AppStateV1::magicKey) {
        aquarium_lamp::AppStateV1 origState;
        storage->read(&origState, sizeof(AppStateV1), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AppStateV2::migrateFrom(aquarium_lamp::AppStateV1 *origState)
{
    MqttAppStateV1::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
}
