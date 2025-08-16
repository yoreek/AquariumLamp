#include "MqttApi.h"
#include "../App.h"
#include "../AppStateV7.h"

namespace aquarium_lamp {
extern AppStateV7 appState;
extern App app;

void MqttApi::begin() const
{
    _webServer.getServer().on(
        "/api/mqtt", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<1024> doc;

            const auto &mqtt = appState.mqtt;
            doc["enabled"] = mqtt.enabled;
            doc["ip"] = IPAddress(mqtt.ip).toString();
            doc["port"] = mqtt.port;
            doc["clientId"].set(JsonString(mqtt.clientId, JsonString::Linked));
            doc["username"].set(JsonString(mqtt.username, JsonString::Linked));
            doc["password"].set(JsonString(mqtt.password, JsonString::Linked));

            rd::WebServerManager::sendSuccess(request, doc);
        });

    _webServer.getServer().on(
        "/api/mqtt", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<1024> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            const auto mqtt = &appState.mqtt;
            const auto obj = doc.as<JsonObject>();
            _assignBool(obj, "enabled", mqtt->enabled);
            _assignIP(obj, "ip", mqtt->ip);
            if (obj.containsKey("port")) {
                mqtt->port = obj["port"].as<uint16_t>();
            }
            _assignString(obj, "clientId", mqtt->clientId);
            _assignString(obj, "username", mqtt->username);
            _assignString(obj, "password", mqtt->password);
            appState.updated = true;
            rd::WebServerManager::sendSuccess(request);
        }
    );
}

}
