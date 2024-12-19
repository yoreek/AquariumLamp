#include "AquariumLampAppStateV4.h"
#include <debug/Debug.h>

bool AquariumLampAppStateV4::isUpdated() const
{
    return MqttAppStateV3::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV4::markAsSaved()
{
    MqttAppStateV3::markAsSaved();
    lamp.markAsSaved();
}

void AquariumLampAppStateV4::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
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
    else if (prevMagicKey == AquariumLampAppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        AquariumLampAppStateV3 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV3), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AquariumLampAppStateV4::migrateFrom(AquariumLampAppStateV1 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AquariumLampAppStateV4::migrateFrom(AquariumLampAppStateV2 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}

void AquariumLampAppStateV4::migrateFrom(AquariumLampAppStateV3 *origState)
{
    MqttAppStateV3::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
