#include <Arduino.h>
#include "debug/Debug.h"
#include "AquariumLampApp.h"
#include "AquariumLampVersion.h"
#include <version/ReefDuinoVersion.h>

REEFDUINO_NAMESPACE_USING

AquariumLampApp app;

void setup()
{
    DEBUG_BEGIN
    LOG_INFO("Project version: %s", AquariumLampVersion.full);
    LOG_INFO("ReefDuino version: %s", ReefDuinoVersion.full);
    app.begin();
}

void loop()
{
    DEBUG_LOOP
    app.loop();
}
