#include "AquariumLampAppStateV2.h"

bool AquariumLampAppStateV2::isUpdated() const
{
    return MqttAppStateV1::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV2::markAsSaved()
{
    MqttAppStateV1::markAsSaved();
    lamp.markAsSaved();
}

void AquariumLampAppStateV2::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == AquariumLampAppStateV1::magicKey) {
        AquariumLampAppStateV1 origState;
        storage->read(&origState, sizeof(AquariumLampAppStateV1), sizeof(prevMagicKey));
        migrateFrom(&origState);
    }
}

void AquariumLampAppStateV2::migrateFrom(AquariumLampAppStateV1 *origState)
{
    MqttAppStateV1::migrateFrom(origState);
    lamp.migrateFrom(&origState->lamp);
}
