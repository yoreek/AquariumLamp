#include "LampApi.h"

#include "../App.h"
#include "../AppStateV7.h"

namespace aquarium_lamp {
extern AppStateV7 appState;
extern App app;

const char LampApi::Scheduled[] = "scheduled";
const char LampApi::Manual[] = "manual";

void LampApi::begin() const
{
    _webServer.getServer().on(
        "/api/lamp/state", HTTP_GET, [](AsyncWebServerRequest* request){
            StaticJsonDocument<2048> doc;

            doc["mode"] = appState.lamp.mode == ScheduledLedArrayMode::ManualMode ? Manual : Scheduled;

            const JsonArray br = doc.createNestedArray("manualBrightness");
            for (int i = 0; i < 5; ++i) {
                br.add(static_cast<int8_t>(round(app.getManualBrightness(i) * 100)));
            }

            const JsonArray schedules = doc.createNestedArray("schedules");
            for (const auto & schedule : appState.lamp.schedules) {
                JsonObject scheduleObj = schedules.createNestedObject();
                scheduleObj["enabled"] = schedule.enabled;
                char buf[16];
                schedule.from.iso8601(buf, false);
                scheduleObj["from"] = buf;

                JsonArray brArray = scheduleObj.createNestedArray("brightness");
                for (const float brightness : schedule.brightness) {
                    static_cast<void>(brArray.add(static_cast<int>(round(brightness * 100.0f))));
                }
            }

            rd::WebServerManager::sendSuccess(request, doc);
        }
    );

    _webServer.getServer().on(
        "/api/lamp/mode", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
            StaticJsonDocument<256> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Malformed JSON");
                return;
            }
            const char *mode = doc["mode"];
            if (!mode) {
                rd::WebServerManager::sendError(request, 400, "MISSING_FIELD",
                                                "Field 'mode' is required");
                return;
            }
            if (!_setLampMode(mode)) {
                rd::WebServerManager::sendError(request, 400, "INVALID_VALUE",
                                                "Unsupported mode");
                return;
            }
            rd::WebServerManager::sendSuccess(request);
        }
    );

    _webServer.getServer().on(
        "/api/lamp/brightness", HTTP_POST,
        rd::WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<256> doc;
            if (auto err = deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Malformed JSON");
                return;
            }
            if (!doc.containsKey("channel") || !doc.containsKey("brightness")) {
                rd::WebServerManager::sendError(request, 400, "MISSING_FIELD", "Fields 'channel' and 'brightness' are required");
                return;
            }
            const int channel = doc["channel"];
            const int brightness = doc["brightness"];
            if (channel < 0 || channel >= 5) {
                rd::WebServerManager::sendError(request, 400, "OUT_OF_RANGE", "Channel must be 0..4");
                return;
            }
            if (brightness < 0 || brightness > 100) {
                rd::WebServerManager::sendError(request, 400, "OUT_OF_RANGE", "Brightness must be 0..100");
                return;
            }
            if (!app.setManualBrightness(channel, static_cast<float>(brightness) / 100.0f)) {
                rd::WebServerManager::sendError(request, 500, "APPLY_FAILED", "Failed to set brightness");
                return;
            }
            rd::WebServerManager::sendSuccess(request);
        }
    );
    _webServer.getServer().on(
        "/api/lamp/schedules", HTTP_GET, [](AsyncWebServerRequest* req){
        StaticJsonDocument<1024> doc;
        const JsonArray points=doc.createNestedArray("schedules");
        for (const auto & schedule : appState.lamp.schedules) {
            JsonObject pt = points.createNestedObject();
            pt["enabled"] = schedule.enabled;
            char buf[16];
            schedule.from.iso8601(buf, false);
            pt["from"] = buf;
            JsonArray br = pt.createNestedArray("brightness");
            for (const float brightness : schedule.brightness) {
                static_cast<void>(br.add(static_cast<int>(round(brightness * 100.0f))));
            }
        }
        rd::WebServerManager::sendSuccess(req, doc);
    });

    _webServer.getServer().on(
        "/api/lamp/schedules", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t, size_t){
            StaticJsonDocument<512> doc;
            if (auto err=deserializeJson(doc,data,len)){
                rd::WebServerManager::sendError(req,400,"BAD_JSON","Malformed JSON");
                return;
            }
            if (!doc.containsKey("index") || !doc.containsKey("schedule")){
                rd::WebServerManager::sendError(req,400,"MISSING_FIELD","index and point required");
                return;
            }
            const int index=doc["index"];
            if (index < 0 || index >= std::size(appState.lamp.schedules)) {
                rd::WebServerManager::sendError(req,400,"OUT_OF_RANGE","index must be 0..9");
                return;
            }
            const auto scheduleNode = doc["schedule"].as<JsonObject>();
            if (!scheduleNode.containsKey("enabled")
                || !scheduleNode.containsKey("from")
                || !scheduleNode.containsKey("brightness")) {
                rd::WebServerManager::sendError(req,400,"MISSING_FIELD","point must have enabled, time, brightness");
                return;
            }

            const char* str = scheduleNode["from"];
            const auto from = rd::Time(str, strlen(str));
            appState.lamp.schedules[index].enabled = scheduleNode["enabled"].as<bool>();
            appState.lamp.schedules[index].from = from;

            const JsonArray brNode = scheduleNode["brightness"].as<JsonArray>();
            if (brNode.size() != std::size(appState.lamp.schedules[index].brightness)) {
                rd::WebServerManager::sendError(req,400,"INVALID_VALUE","brightness must have 5 elements");
                return;
            }
            for (int i = 0; i < std::size(appState.lamp.schedules[index].brightness); ++i) {
                const int brightness= brNode[i];
                if (brightness < 0 || brightness > 100){
                    rd::WebServerManager::sendError(req,400,"OUT_OF_RANGE","brightness values must be 0..100");
                    return;
                }
                appState.lamp.schedules[index].brightness[i] = static_cast<float>(brightness) / 100.0f;
            }
            appState.updated = true;

            doc.clear();
            doc["index"] = index;
            rd::WebServerManager::sendSuccess(req, doc);
        }
    );
}

bool LampApi::_setLampMode(const char *mode) {
    if (strcmp(mode, Manual) == 0) {
        appState.lamp.mode = ScheduledLedArrayMode::ManualMode;
    } else if (strcmp(mode, Scheduled) == 0) {
        appState.lamp.mode = ScheduledLedArrayMode::ScheduledMode;
    } else {
        return false; // Unsupported mode
    }
    appState.updated = true;
    return true;
}
}
