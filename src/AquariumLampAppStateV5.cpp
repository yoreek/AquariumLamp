#include "AquariumLampAppStateV5.h"
#include <debug/Debug.h>

bool AquariumLampAppStateV5::isUpdated() const
{
    return MqttAppStateV3::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV5::markAsSaved()
{
    MqttAppStateV3::markAsSaved();
    lamp.markAsSaved();
}

void AquariumLampAppStateV5::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == AquariumLampAppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        AquariumLampAppStateV1 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV1), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        AquariumLampAppStateV2 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV2), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        AquariumLampAppStateV3 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV3), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV4::magicKey) {
        LOG_INFO("migrate from V4");
        AquariumLampAppStateV4 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV4), sizeof(prevMagicKey));
        MqttAppStateV3::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
}
