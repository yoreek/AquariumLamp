#include "NtpApi.h"
#include "../App.h"
#include "../AppStateV6.h"

namespace aquarium_lamp {
extern AppStateV6 appState;
extern App app;

void NtpApi::begin() const
{
    _webServer.getServer().on(
        "/api/ntp", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<512> doc;

            doc["server"] = appState.ntp.server;
            doc["syncInterval"] = appState.ntp.syncInterval / 1000 ;
            doc["timezone"] = appState.timezone;

            rd::WebServerManager::sendSuccess(request, doc);
        });

    _webServer.getServer().on(
        "/api/ntp", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<512> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            const auto obj = doc.as<JsonObject>();
            _assignString(obj, "server", appState.ntp.server);
            if (obj.containsKey("syncInterval")) {
                appState.ntp.syncInterval = obj["syncInterval"].as<uint32_t>() * 1000; // Convert seconds to milliseconds
            }
            if (_assignString(obj, "timezone", appState.timezone)) {
                app.initTimezone();
            }
            appState.updated = true;
            rd::WebServerManager::sendSuccess(request);
        }
    );
}
}
