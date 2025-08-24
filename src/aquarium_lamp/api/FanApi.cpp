#include "FanApi.h"
#include "../App.h"
#include "../AppStateV7.h"

namespace aquarium_lamp {
extern AppStateV7 appState;
extern App app;

const char FanApi::AutoMode[] = "auto";
const char FanApi::OffMode[] = "off";
const char FanApi::OnMode[] = "on";
const char FanApi::PauseMode[] = "pause";

void FanApi::begin() const
{
    _webServer.getServer().on(
        "/api/fan/thermostat", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<256> doc;

            doc["temperature"] = appState.overheatingSensor.targetValue;
            doc["hysteresis"] = appState.overheatingSensor.hysteresis;

            rd::WebServerManager::sendSuccess(request, doc);
        });

    _webServer.getServer().on(
        "/api/fan/thermostat", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<256> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            if (doc.containsKey("temperature")) {
                const auto temp = doc["temperature"].as<float>();
                if (temp < 0 || temp > 100) {
                    rd::WebServerManager::sendError(request, 400, "BAD_TEMPERATURE", "Temperature must be between 0 and 100"); return;
                }
                appState.overheatingSensor.targetValue = temp;
            }
            if (doc.containsKey("hysteresis")) {
                const auto hysteresis = doc["hysteresis"].as<float>();
                if (hysteresis < 1 || hysteresis > 10) {
                    rd::WebServerManager::sendError(request, 400, "BAD_HYSTERESIS", "Hysteresis must be between 1 and 10"); return;
                }
                appState.overheatingSensor.hysteresis = hysteresis;
            }
            appState.updated = true;
            rd::WebServerManager::sendSuccess(request);
        }
    );

    _webServer.getServer().on(
        "/api/fan/state", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<128> doc;

            switch (appState.fanSwitch.mode) {
                case AutoSwitchMode::AutoMode:
                    doc["mode"] = AutoMode;
                    break;
                case AutoSwitchMode::TurnedOffMode:
                    doc["mode"] = OffMode;
                    break;
                case AutoSwitchMode::TurnedOnMode:
                    doc["mode"] = OnMode;
                    break;
                case AutoSwitchMode::PausedMode:
                    doc["mode"] = PauseMode;
                    break;
            }

            rd::WebServerManager::sendSuccess(request, doc);
        });

    _webServer.getServer().on(
        "/api/fan/state", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<128> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            const char *mode = doc["mode"];
            if (!mode) {
                rd::WebServerManager::sendError(request, 400, "MISSING_FIELD",
                                                "Field 'mode' is required");
                return;
            }
            if (!_setFanMode(mode)) {
                rd::WebServerManager::sendError(request, 400, "INVALID_VALUE",
                                                "Unsupported mode");
                return;
            }
            appState.updated = true;
            rd::WebServerManager::sendSuccess(request);
        }
    );
}

bool FanApi::_setFanMode(const char *mode) {
    if (strcmp(mode, OffMode) == 0) {
        appState.fanSwitch.mode = AutoSwitchMode::TurnedOffMode;
    } else if (strcmp(mode, OnMode) == 0) {
        appState.fanSwitch.mode = AutoSwitchMode::TurnedOnMode;
    } else if (strcmp(mode, AutoMode) == 0) {
        appState.fanSwitch.mode = AutoSwitchMode::AutoMode;
    } else if (strcmp(mode, PauseMode) == 0) {
        appState.fanSwitch.mode = AutoSwitchMode::PausedMode;
    } else {
        return false; // Unsupported mode
    }
    return true;
}

}
