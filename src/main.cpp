#include <Arduino.h>
#include "debug/Debug.h"
#include "AquariumLampApp.h"

REEFDUINO_NAMESPACE_USING

AquariumLampApp app;

void setup()
{
    DEBUG_BEGIN
    LOG_INFO("begin");
    app.begin();
}

void loop()
{
    DEBUG_LOOP
    app.loop();
}
