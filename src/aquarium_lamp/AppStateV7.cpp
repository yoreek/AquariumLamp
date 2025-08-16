#include "AppStateV7.h"
#include <debug/Debug.h>
#include "AppStateV1.h"
#include "AppStateV2.h"
#include "AppStateV3.h"
#include "AppStateV4.h"
#include "AppStateV5.h"
#include "AppStateV6.h"

namespace aquarium_lamp {
bool AppStateV7::isUpdated() const
{
    return MqttAppStateV5::isUpdated() || lamp.isUpdated();
}

void AppStateV7::markAsSaved()
{
    MqttAppStateV5::markAsSaved();
    lamp.markAsSaved();
}

void AppStateV7::migrate(AbstractStorage *storage, uint32_t prevMagicKey)
{
    if (prevMagicKey == aquarium_lamp::AppStateV1::magicKey) {
        LOG_INFO("migrate from V1");
        aquarium_lamp::AppStateV1 origState;
        storage->read(&origState, sizeof(aquarium_lamp::AppStateV1), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == aquarium_lamp::AppStateV2::magicKey) {
        LOG_INFO("migrate from V2");
        aquarium_lamp::AppStateV2 origState;
        storage->read(&origState, sizeof(AppStateV2), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV3::magicKey) {
        LOG_INFO("migrate from V3");
        AppStateV3 origState;
        storage->read(&origState, sizeof(AppStateV3), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV4::magicKey) {
        LOG_INFO("migrate from V4");
        AppStateV4 origState;
        storage->read(&origState, sizeof(AppStateV4), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV5::magicKey) {
        LOG_INFO("migrate from V5");
        AppStateV5 origState;
        storage->read(&origState, sizeof(AppStateV5), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV6::magicKey) {
        LOG_INFO("migrate from V6");
        AppStateV6 origState;
        storage->read(&origState, sizeof(AppStateV6), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
    else if (prevMagicKey == AppStateV7::magicKey) {
        LOG_INFO("migrate from V7");
        AppStateV7 origState;
        storage->read(&origState, sizeof(AppStateV7), sizeof(prevMagicKey));
        MqttAppStateV5::migrateFrom(&origState);
        lamp.migrateFrom(&origState.lamp);
    }
}
}
