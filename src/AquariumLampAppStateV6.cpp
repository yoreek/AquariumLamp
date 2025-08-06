#include "AquariumLampAppStateV6.h"
#include <debug/Debug.h>
#include "AquariumLampAppStateV1.h"
#include "AquariumLampAppStateV2.h"
#include "AquariumLampAppStateV3.h"
#include "AquariumLampAppStateV4.h"
#include "AquariumLampAppStateV5.h"

bool AquariumLampAppStateV6::isUpdated() const
{
    return MqttAppStateV4::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV6::markAsSaved()
{
    MqttAppStateV4::markAsSaved();
    lamp.markAsSaved();
}

void AquariumLampAppStateV6::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == AquariumLampAppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        AquariumLampAppStateV1 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV1), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        AquariumLampAppStateV2 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV2), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        AquariumLampAppStateV3 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV3), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV4::magicKey) {
        LOG_INFO("migrate from V4");
        AquariumLampAppStateV4 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV4), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AquariumLampAppStateV5::magicKey) {
        LOG_INFO("migrate from V5");
        AquariumLampAppStateV5 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV5), sizeof(prevMagicKey));
        MqttAppStateV4::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
}
