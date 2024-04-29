#include "utils.hpp"

#if defined(GEODE_IS_WINDOWS)

void basementutils::patchString(uintptr_t const absAddress, char const* str) {
    Mod::get()->patch((void*)absAddress, ByteVector {(uint8_t*)&str, (uint8_t*)&str + 4});
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

#elif defined(GEODE_IS_ANDROID)

void basementutils::patchString(uintptr_t const dcd, uintptr_t const add, char const* str) {
    Mod::get()->patch((void*)(base::get() + dcd), ByteVector {(uint8_t*)&str, (uint8_t*)&str + 4});
    Mod::get()->patch((void*)(base::get() + add), ByteVector {0x00, 0xBF});
}

#endif

std::string const basementutils::getVersion() {
    return fmt::format("{}.{}.{}", ver.major, ver.minor, ver.revision);
}

std::string const basementutils::getQualityString(std::string filename) {
    if(filename.find("-uhd") != std::string::npos) filename.erase(filename.find("-uhd"), 4);
    if(filename.find("-hd") != std::string::npos) filename.erase(filename.find("-hd"), 3);

    std::string extension = ghc::filesystem::path(filename).extension().string();
    auto quality = CCDirector::get()->getLoadedTextureQuality();

    switch (quality){
        case TextureQuality::kTextureQualityLow:
            break;

        case TextureQuality::kTextureQualityMedium:
            filename.replace(filename.find(extension), std::string("-hd" + extension).length(), "-hd" + extension);
            break;

        case TextureQuality::kTextureQualityHigh:
            filename.replace(filename.find(extension), std::string("-uhd" + extension).length(), "-uhd" + extension);
            break;  
    }

    return filename;
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

    auto url = fmt::format("{}://{}/{}", https ? "https" : "http", help, prefix ? (setting == "main" ? "pgcore" : "server") : "");

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