#include "OneWireDeviceApi.h"
#include "../App.h"

namespace aquarium_lamp {
extern App app;

void OneWireDeviceApi::begin() const
{
    _webServer.getServer().on(
        "/api/one_wire_device/scan", HTTP_GET,
        [](AsyncWebServerRequest* request){
            const auto &scanner = app.getOneWireDeviceScanner();

            StaticJsonDocument<1024> doc;
            doc["inProgress"] = scanner.inProgress();
            doc["completed"] = scanner.completed();
            const JsonArray arr = doc.createNestedArray("devices");
            const auto *devices = scanner.devices();
            if (!devices) {
                rd::WebServerManager::sendSuccess(request, doc);
                return;
            }

            const auto count  = scanner.devicesCount();
            for (size_t i = 0; i < count; i++) {
                char addr[3 * 8 + 1] = {0};
                TempApi::addressToString(devices[i], addr);
                arr.add(addr);
            }
            rd::WebServerManager::sendSuccess(request, doc);
        });

    // start scan
    _webServer.getServer().on(
        "/api/one_wire_device/scan", HTTP_POST,
        [](AsyncWebServerRequest* request) {
            app.getOneWireDeviceScanner().start();
            rd::WebServerManager::sendSuccess(request);
        },nullptr, nullptr
    );
}

}
