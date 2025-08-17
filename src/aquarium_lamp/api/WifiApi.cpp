#include "WifiApi.h"
#include "../App.h"
#include "../AppStateV7.h"

namespace aquarium_lamp {
extern AppStateV7 appState;
extern App app;

void WifiApi::begin() const
{
    // WiFi scan
    _webServer.getServer().on(
        "/api/wifi/scan", HTTP_GET,
        [](AsyncWebServerRequest* request){
            WifiScanner &scanner = app.wifi().getScanner();

            StaticJsonDocument<1024> doc;
            doc["scanning"] = scanner.isScanning();
            char buf[32];
            scanner.lastUpdatedAt().iso8601(buf);
            doc["lastUpdatedAt"] = buf;

            const JsonArray arr = doc.createNestedArray("networks");
            const auto& nets = scanner.getNetworks();
            for (const auto &[ssid, signal, secure] : nets) {
                JsonObject obj = arr.createNestedObject();
                obj["ssid"] = ssid;
                obj["signal"] = signal;
                obj["secure"] = secure;
            }
            rd::WebServerManager::sendSuccess(request, doc);
        });

    // start scan
    _webServer.getServer().on(
        "/api/wifi/scan", HTTP_POST,
        [](AsyncWebServerRequest* request) {
            WifiScanner &scanner = app.wifi().getScanner();
            scanner.startScan();
            rd::WebServerManager::sendSuccess(request);
        },nullptr, nullptr
    );

    // Get current WiFi config
    _webServer.getServer().on(
        "/api/wifi/config", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            StaticJsonDocument<1024> doc;

            const auto staArray = doc.createNestedArray("sta");
            for (const auto & config : appState.wifi.sta) {
                const auto staNode = staArray.createNestedObject();
                staNode["enabled"] = config.enabled;
                staNode["ssid"].set(JsonString(config.ssid, JsonString::Linked));
                staNode["password"].set(JsonString(config.password, JsonString::Linked));
                staNode["hostname"].set(JsonString(config.hostname, JsonString::Linked));
                staNode["ip"] = IPAddress(config.ip).toString();
                staNode["gateway"] = IPAddress(config.gateway).toString();
                staNode["subnet"] = IPAddress(config.subnet).toString();
                staNode["dns1"] = IPAddress(config.dns1).toString();
                staNode["dns2"] = IPAddress(config.dns2).toString();
                staNode["dhcp"] = config.dhcp;
                staNode["autoDns"] = config.autoDns;
            }

            const auto apNode = doc.createNestedObject("ap");
            const auto &ap = appState.wifi.ap;
            apNode["enabled"] = ap.enabled;
            apNode["ssid"].set(JsonString(ap.ssid, JsonString::Linked));
            apNode["password"].set(JsonString(ap.password, JsonString::Linked));
            apNode["ip"] = IPAddress(ap.ip).toString();
            apNode["gateway"] = IPAddress(ap.gateway).toString();
            apNode["subnet"] = IPAddress(ap.subnet).toString();

            rd::WebServerManager::sendSuccess(request, doc);
        });

    // Update WiFi config
    _webServer.getServer().on(
        "/api/wifi/config", HTTP_POST,
        WebServerManager::blankCallback, nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            StaticJsonDocument<1024> doc;
            if (deserializeJson(doc, data, len)) {
                rd::WebServerManager::sendError(request, 400, "BAD_JSON", "Bad JSON"); return;
            }

            if (doc.containsKey("sta")) {
                _updateStaSettings(doc["sta"].as<JsonArray>());
            }
            if (doc.containsKey("ap")) {
                _updateApSettings(doc["ap"].as<JsonObject>());
            }
            appState.updated = true;
            rd::WebServerManager::sendSuccess(request);
        }
    );
}

bool WifiApi::_updateStaSettings(const JsonArray &arr)
{
    for (uint8_t i = 0; i < arr.size(); i++) {
        if (i >= std::size(appState.wifi.sta)) break;

        auto staNode = arr[i].as<JsonObject>();
        auto * sta = &appState.wifi.sta[i];
        _assignBool(staNode, "enabled", sta->enabled);
        _assignString(staNode, "ssid", sta->ssid);
        _assignString(staNode, "password", sta->password);
        _assignString(staNode, "hostname", sta->hostname);
        _assignIP(staNode, "ip", sta->ip);
        _assignIP(staNode, "gateway", sta->gateway);
        _assignIP(staNode, "subnet", sta->subnet);
        _assignIP(staNode, "dns1", sta->dns1);
        _assignIP(staNode, "dns2", sta->dns2);
        _assignBool(staNode, "dhcp", sta->dhcp);
        _assignBool(staNode, "autoDns", sta->autoDns);
    }

    return true;
}

void WifiApi::_updateApSettings(const JsonObject &node)
{
    auto * ap = &appState.wifi.ap;
    _assignBool(node, "enabled", ap->enabled);
    _assignString(node, "ssid", ap->ssid);
    _assignString(node, "password", ap->password);
    _assignIP(node, "ip", ap->ip);
    _assignIP(node, "gateway", ap->gateway);
    _assignIP(node, "subnet", ap->subnet);
}
}
