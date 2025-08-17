#include "BaseApi.h"

namespace aquarium_lamp {
BaseApi::BaseApi(WebServerManager& webServer)
    : _webServer(webServer)
{
}

bool BaseApi::_assignIP(const JsonObject &obj, const char *key, uint8_t (&dst)[4])
{
    if (!obj.containsKey(key)) return false;
    if (IPAddress ipAddr; ipAddr.fromString(obj[key].as<const char *>())) {
        for (int i = 0; i < 4; ++i) dst[i] = ipAddr[i];
        return true;
    }
    return false;
}

bool BaseApi::_assignBool(const JsonObject &obj, const char *key, bool &dst)
{
    if (obj.containsKey(key)) {
        dst = obj[key].as<bool>();
        return true;
    }
    return false;
}
}
