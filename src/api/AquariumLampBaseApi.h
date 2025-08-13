#pragma once
#include "net/WebServerManager.h"

REEFDUINO_NAMESPACE_USING

class AquariumLampBaseApi {
public:
    explicit AquariumLampBaseApi(WebServerManager& webServer);
    virtual ~AquariumLampBaseApi() = default;
    virtual void begin() const = 0;

protected:
    WebServerManager& _webServer;

    template <size_t Size>
    static bool _assignString(const JsonObject &obj, const char *key, char (&dst)[Size])
    {
        if (obj.containsKey(key)) {
            strlcpy(dst, obj[key].as<const char *>(), Size);
            return true;
        }
        return false;
    }
    static bool _assignIP(const JsonObject &obj, const char *key, uint8_t (&dst)[4]);
    static bool _assignBool(const JsonObject &obj, const char *key, bool &dst);
};
