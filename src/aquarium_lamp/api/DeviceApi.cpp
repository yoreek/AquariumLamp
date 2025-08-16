#include "DeviceApi.h"
#include "Version.h"
#include "../App.h"
#include "../AppStateV6.h"
#include "time/TimeUtil.h"

namespace aquarium_lamp {
extern AppStateV6 appState;
extern App app;

void DeviceApi::begin() const
{
    _webServer.getServer().on("/api/device/info", HTTP_GET, [this](AsyncWebServerRequest* request) {
        StaticJsonDocument<512> doc;
        char buf[50];

        doc["firmware"] = aquarium_lamp::Version.full;

        // IP: берём текущий локальный IP (если подключены), иначе AP
        String ipStr = "0.0.0.0";
        if (WiFi.status() == WL_CONNECTED) {
            ipStr = WiFi.localIP().toString();
            doc["rssi"] = WiFi.RSSI();
            doc["subnet"] = WiFi.subnetMask().toString();
            doc["gateway"] = WiFi.gatewayIP().toString();
        } else {
            ipStr = WiFi.softAPIP().toString();
        }
        doc["ip"] = ipStr;

        byte mac[6];
        WiFi.macAddress(mac);
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        doc["macAddress"] = buf;

        doc["heapSize"] = ESP.getHeapSize();
        doc["heapUsage"] = (ESP.getHeapSize() - ESP.getFreeHeap()) * 100 / ESP.getHeapSize();
        doc["maxFreeBlockSize"] = ESP.getMaxAllocHeap();

        const uint32_t uptimeSecs = millis() / 1000;
        // 365 day(s) 23 hour(s) 59 minute(s) 59 second(s)
        TimeDelta(static_cast<int32_t>(uptimeSecs)).toString(buf);
        doc["uptime"] = buf;

        const auto currentTime = DateTime::current();
        currentTime.iso8601(buf);
        doc["time"] = buf;

        doc["timezone"] = appState.timezone;

        rd::WebServerManager::sendSuccess(request, doc);
    });

    _webServer.getServer().on(
        "/api/device/reboot", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            LOG_DEBUG("reboot");
            app.reboot();
            rd::WebServerManager::sendSuccess(request);
        }, nullptr, nullptr
    );

    _webServer.getServer().on(
        "/api/device/factory_reset", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            LOG_DEBUG("factory reset");
            app.factoryReset();
            rd::WebServerManager::sendSuccess(request);
        }, nullptr, nullptr
    );

    // { "date": "2025-08-13T18:22:00" }
    _webServer.getServer().on(
        "/api/device/time", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<256> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            char date[32] = {0};
            if (const auto obj = doc.as<JsonObject>(); !_assignString(obj, "date", date)) {
                rd::WebServerManager::sendError(request, 400, "MISSING_FIELD",
                                                "Fields 'date' and 'time' are required");
                return;
            }
            const DateTime dt(date, strlen(date));
            if (!dt.isValid() || dt.isNull()) {
                rd::WebServerManager::sendError(request, 400, "INVALID_DATE_TIME",
                                                "Invalid date or time format");
                return;
            }

            const uint32_t utc = systemTimezone.toUTC(dt.unixtime());
            setTime(utc);

            rd::WebServerManager::sendSuccess(request);
        }
    );
}
}
