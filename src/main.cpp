#include <Arduino.h>
#include "debug/Debug.h"
#include "aquarium_lamp/App.h"
#include "Version.h"
#include <version/ReefDuinoVersion.h>

REEFDUINO_NAMESPACE_USING

namespace aquarium_lamp {
aquarium_lamp::App app;
};

void setup()
{
    DEBUG_BEGIN
    LOG_INFO("Project version: %s", aquarium_lamp::Version.full);
    LOG_INFO("ReefDuino version: %s", ReefDuinoVersion.full);
    aquarium_lamp::app.begin();
}

void loop()
{
    DEBUG_LOOP
    aquarium_lamp::app.loop();
}
