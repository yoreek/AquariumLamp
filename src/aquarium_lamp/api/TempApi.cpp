#include "TempApi.h"
#include "../App.h"
#include "../AppStateV7.h"

namespace aquarium_lamp {
extern AppStateV7 appState;
extern App app;

void TempApi::begin() const
{
    _webServer.getServer().on(
        "/api/temp/config", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<512> doc;

            const auto &tempSensor = &appState.tempSensor;
            char buf[3 * 8];
            addressToString(tempSensor->address, buf);
            doc["address"] = buf;
            doc["exp_filter_wight"] = tempSensor->expFilter.weight;
            doc["approx_factor"] = tempSensor->approxFilter.factor;
            doc["approx_offset"] = tempSensor->approxFilter.offset;

            rd::WebServerManager::sendSuccess(request, doc);
        });

    _webServer.getServer().on(
        "/api/temp/config", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<512> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            if (doc.containsKey("address") && doc["address"].is<const char*>()) {
                const auto addr = doc["address"].as<const char*>();
                byte arr[8] = {0};
                if (parseAddress(addr, arr)) {
                    memcpy(appState.tempSensor.address, arr, sizeof(arr));
                    appState.updated = true;
                } else {
                    rd::WebServerManager::sendError(request, 400, "BAD_ADDRESS", "Invalid address format"); return;
                }
            }
            if (doc.containsKey("exp_filter_weight")) {
                const auto weight = doc["exp_filter_weight"].as<float>();
                if (weight < 0 || weight > 1) {
                    rd::WebServerManager::sendError(request, 400, "BAD_WEIGHT", "Weight must be between 0 and 1"); return;
                }
                appState.tempSensor.expFilter.weight = weight;
                appState.updated = true;
            }
            if (doc.containsKey("approx_factor")) {
                const auto factor = doc["approx_factor"].as<float>();
                if (factor < 0.1 || factor > 10) {
                    rd::WebServerManager::sendError(request, 400, "BAD_FACTOR", "Factor must be between 0.1 and 10"); return;
                }
                appState.updated = true;
                appState.tempSensor.approxFilter.factor = factor;
            }
            if (doc.containsKey("approx_offset")) {
                const auto offset = doc["approx_offset"].as<float>();
                if (offset < -10 || offset > 10) {
                    rd::WebServerManager::sendError(request, 400, "BAD_OFFSET", "Offset must be between -10 and 10"); return;
                }
                appState.updated = true;
                appState.tempSensor.approxFilter.offset = offset;
            }

            rd::WebServerManager::sendSuccess(request);
        }
    );

    _webServer.getServer().on(
        "/api/temp/current", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<128> doc;
            auto tempSensor = app.getTempSensor();
            doc["ready"] = tempSensor.isReady();
            if (tempSensor.isReady()) {
                doc["temperature"] = tempSensor.getValue();
            }
            rd::WebServerManager::sendSuccess(request, doc);
        });
}

bool TempApi::parseAddress(const char *addr, byte *arr)
{
    if (!addr || !arr) {
        return false;
    }
    int i = 0;
    char *end;
    while (i < 8 && *addr) {
        arr[i] = static_cast<byte>(strtol(addr, &end, 16));
        if (end == addr || (*end != '\0' && *end != ':')) {
            return false; // Invalid format
        }
        addr = end + 1; // Move past the colon
        i++;
    }
    return i == 8; // Ensure we read exactly 8 bytes
}

void TempApi::addressToString(const byte* address, char *buf) {
    int pos = 0;
    for (int i = 0; i < 8; ++i) {
        if (i > 0) buf[pos++] = ':';
        sprintf(buf + pos, "%02X", address[i]);
        pos += 2;
    }
    buf[pos] = '\0';
}
}
