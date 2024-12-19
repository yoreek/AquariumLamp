#include "AquariumLampAppStateV3.h"
#include <debug/Debug.h>

bool AquariumLampAppStateV3::isUpdated() const
{
    return MqttAppStateV2::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV3::markAsSaved()
{
    MqttAppStateV2::markAsSaved();
    lamp.markAsSaved();
}

void AquariumLampAppStateV3::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == AquariumLampAppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        AquariumLampAppStateV1 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV1), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
    else if (prevMagicKey == AquariumLampAppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        AquariumLampAppStateV2 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV2), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AquariumLampAppStateV3::migrateFrom(AquariumLampAppStateV1 *origState)
{
    MqttAppStateV2::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AquariumLampAppStateV3::migrateFrom(AquariumLampAppStateV2 *origState)
{
    MqttAppStateV2::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
