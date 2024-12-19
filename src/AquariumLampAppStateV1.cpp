#include "AquariumLampAppStateV1.h"

bool AquariumLampAppStateV1::isUpdated() const
{
    return MqttAppStateV1::isUpdated() || lamp.isUpdated();
}

void AquariumLampAppStateV1::markAsSaved()
{
    MqttAppStateV1::markAsSaved();
    lamp.markAsSaved();
}

//AquariumLampAppStateV1 appStateV1 {
//    {
//        {
//            {
//                .dstTimeChangeRule {"WITA", First, Sun, Mar, 2, 8 * 60},
//                .stdTimeChangeRule {"WITA", First, Sun, Mar, 2, 8 * 60}
//            },
//            .wifi = {
//                {
//                    .ssid = "YoreekGX",
//                    .password = "abrakadabra",
//                    .hostname = "aquarium-lamp2",
//                    .ip = {192, 168, 1, 169},
//                    .gateway = {192, 168, 1, 1},
//                    .subnet = {255, 255, 255, 0},
//                    .dns1 = {8, 8, 8, 8},
//                    .dns2 = {8, 8, 4, 4}
//                },
//                {
//                    .ssid = "Abasan82",
//                    .password = "abrakadabra",
//                    .hostname = "aquarium-lamp2",
//                    .ip = {192, 168, 1, 169},
//                    .gateway = {192, 168, 1, 1},
//                    .subnet = {255, 255, 255, 0},
//                    .dns1 = {8, 8, 8, 8},
//                    .dns2 = {8, 8, 4, 4}
//                }
//            },
//            .ntp = {
//                .server = "pool.ntp.org",
//                .syncInterval = 3600000
//            }
//        },
//        .mqtt = {
//            .ip = { 192, 168, 1, 147 },
//            .port = 8883,
//            .clientId = "aquarium_lamp2",
//            .username = "abasan",
//            .password = "abasan"
//        }
//    },
//    .lamp = {
//        .mode = ScheduledLedArrayMode::ManualMode,
//            .schedules = {
//                    {
//                            .from = {Time{(uint8_t) 0, (uint8_t) 0}},
//                            .brightness = {0, 0, 0, 0, 0},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 7, (uint8_t) 0}},
//                            .brightness = {0, 0, 0, 0, 0},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 8, (uint8_t) 0}},
//                            .brightness = {0.26, 0.11, 0.11, 0.16, 0.23},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 9, (uint8_t) 0}},
//                            .brightness = {0.46, 0.18, 0.15, 0.16, 0.44},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 10, (uint8_t) 0}},
//                            .brightness = {1.0, 0.35, 0.36, 0.35, 1.0},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 17, (uint8_t) 0}},
//                            .brightness = {1.0, 0.35, 0.36, 0.35, 1.0},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 18, (uint8_t) 0}},
//                            .brightness = {0.45, 0.24, 0.23, 0.28, 0.43},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 19, (uint8_t) 0}},
//                            .brightness = {0, 0, 0, 0, 0},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 21, (uint8_t) 0}},
//                            .brightness = {0.31, 0, 0, 0, 0.31},
//                            .enabled = true
//                    },
//                    {
//                            .from = {Time{(uint8_t) 23, (uint8_t) 0}},
//                            .brightness = {0.1, 0, 0, 0, 0.1},
//                            .enabled = true
//                    }
//        }
//    }
//};
