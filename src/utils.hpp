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

const std::string basementURL = "https://www.podvalgdsherov.fun/pgcore/";
const Version ver = {1, 5, 0};
inline bool isWinter = false;

namespace basementutils {
    void patchString(uintptr_t const absoluteAddr, char const* str);
    std::string getVersion();
    std::string getQualityString(std::string filename);
    std::string getPrefixQuality(TextureQuality quality);
    bool checkAllQualities(ghc::filesystem::path path);
    const char* getNodeName(CCNode* node);
    std::vector<std::string_view> splitByDelimStringView(std::string_view str, char delim);
    int _stoi(const std::string_view s);
    bool isNumber(const std::string& s);
    bool isFloat(const std::string& str);
    uint32_t GetPointerAddress(std::vector<uint32_t> offsets, const uintptr_t absoluteAddr);
    uint32_t GetPointerAddress(std::vector<uint32_t> offsets, std::string lib);
    uint32_t GetPointerAddress(std::vector<uint32_t> offsets);
    ccColor3B ConvertHSBtoRGB(float h, float s, float v);
    ccColor3B InvertRGBColor(ccColor3B color);
    ccColor3B FadeToColor(ccColor3B source, ccColor3B target, float t);
    std::vector<std::string> splitString(const std::string& str, char separator);

    std::string cp1251_to_utf8(const char *str);
    void getUnicodeChar(unsigned int code, char chars[5]);

    void reloadAll();
};