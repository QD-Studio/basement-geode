#include "utils.hpp"

#if defined(GEODE_IS_WINDOWS)

void basementutils::patchString(uintptr_t const absAddress, char const* str) {
    auto result = Mod::get()->patch((void*)absAddress, ByteVector {(uint8_t*)&str, (uint8_t*)&str + 4});
    if(result.isErr()) {
        log::error("Failed to patch string at 0x{:X}", absAddress);
    }
}

std::string basementutils::cp1251_to_utf8(const char *str){
    std::string res; 
    int result_u, result_c;
    result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if(!result_u ){return 0;}
    wchar_t *ures = new wchar_t[result_u];
    if(!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)){
        delete[] ures;
        return 0;
    }
    result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
    if(!result_c){
        delete [] ures;
        return 0;
    }
    char *cres = new char[result_c];
    if(!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)){
        delete[] cres;
        return 0;
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

#elif defined(GEODE_IS_ANDROID32)

void basementutils::patchString(uintptr_t const dcd, uintptr_t const add, char const* str) {
    Mod::get()->patch((void*)(base::get() + dcd), ByteVector {(uint8_t*)&str, (uint8_t*)&str + 4});
    Mod::get()->patch((void*)(base::get() + add), ByteVector {0x00, 0xBF});
}

#endif

std::string basementutils::pathWithQuality(const std::filesystem::path& path) {
    auto quality = CCDirector::get()->getLoadedTextureQuality();

    auto str = (path.parent_path() / path.stem()).string();
    auto ext = path.extension().string();
    if (auto p = str.find("-uhd"); p != std::string::npos) str.erase(p, 4);
    if (auto p = str.find("-hd"); p != std::string::npos) str.erase(p, 3);

    switch (quality) {
        case TextureQuality::kTextureQualityMedium:
            return str + "-hd" + ext;
        case TextureQuality::kTextureQualityHigh:
            return str + "-uhd" + ext;
        default:
            return str + ext;
    }
}

void basementutils::getUnicodeChar(unsigned int code, char chars[5]) {
    if (code <= 0x7F) {
        chars[0] = (code & 0x7F); chars[1] = '\0';
    }
    else if (code <= 0x7FF) {
        // one continuation byte
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xC0 | (code & 0x1F); chars[2] = '\0';
    }
    else if (code <= 0xFFFF) {
        // two continuation bytes
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xE0 | (code & 0xF); chars[3] = '\0';
    }
    else if (code <= 0x10FFFF) {
        // three continuation bytes
        chars[3] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
        chars[0] = 0xF0 | (code & 0x7); chars[4] = '\0';
    }
    else {
        // unicode replacement character
        chars[2] = 0xEF; chars[1] = 0xBF; chars[0] = 0xBD;
        chars[3] = '\0';
    }
}

void basementutils::reloadAll(){
	CCDirector::sharedDirector()->updateContentScale(CCDirector::get()->getLoadedTextureQuality());

	auto gameManager = GameManager::sharedState();
	// gameManager->setQuality(CCDirector::get()->getLoadedTextureQuality());
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
	gameManager->reloadAll(false, false, true);
}

std::string const basementutils::getServerURL(bool prefix) {
    auto setting = Mod::get()->getSettingValue<std::string>("basement-server");
    std::string help = "www.podvalgdsherov.fun";
    bool https = true;
    if (setting == "local") {
        help = "localhost";
        https = false;
    }

    auto url = fmt::format("{}://{}/{}", https ? "https" : "http", help, prefix ? "server" : "");

    return url;
}

bool basementutils::isWinterNow() {
    auto t = std::time(0);
    tm tm_;
#if defined(GEODE_IS_WINDOWS)
    localtime_s(&tm_, &t);
#elif defined(GEODE_IS_ANDROID)
    localtime_r(&t, &tm_);
#endif

    return tm_.tm_mon <= 1 || tm_.tm_mon == 11;
}