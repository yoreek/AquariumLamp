#include "AquariumLampDeviceApi.h"
#include "AquariumLampVersion.h"
#include "AquariumLampAppStateV6.h"

extern AquariumLampAppStateV6 appState;

AquariumLampDeviceApi::AquariumLampDeviceApi(WebServerManager& webServer)
    : _webServer(webServer)
{
}

void AquariumLampDeviceApi::begin() const
{
    _webServer.getServer().on("/api/device/info", HTTP_GET, [this](AsyncWebServerRequest* request) {
        DynamicJsonDocument doc(512);
        char buf[50];

        doc["firmware"] = AquariumLampVersion.full;

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

        uint32_t uptimeSecs = millis() / 1000;
        // 365 day(s) 23 hour(s) 59 minute(s) 59 second(s)
        TimeDelta(static_cast<int32_t>(uptimeSecs)).toString(buf);
        doc["uptime"] = buf;

        auto currentTime = DateTime::current();
        currentTime.toString(buf);
        doc["time"] = buf;

        doc["timezone"] = appState.timeZoneId;

        _webServer.sendSuccess(request, doc);
    });
}
