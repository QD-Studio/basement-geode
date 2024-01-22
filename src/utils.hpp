#pragma once
#include <string>
#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

struct Version {
    uint16_t major;
    uint16_t minor;
    uint16_t revision;
};

const std::string basementURL = "www.podvalgdsherov.fun";
const Version ver = {1, 6, 0};
inline bool isWinter = false;

namespace basementutils {
#ifdef GEODE_IS_WINDOWS
    void patchString(uintptr_t const absoluteAddr, char const* str);
    std::string cp1251_to_utf8(const char *str);
#elif defined(GEODE_IS_ANDROID)
    void patchString(uintptr_t const dcd, uintptr_t const add, char const* str);
#endif

    const std::string getVersion();
    const std::string getQualityString(std::string filename);
    void getUnicodeChar(unsigned int code, char chars[5]);

    void reloadAll();

    std::string const getServerURL(bool prefix);
};