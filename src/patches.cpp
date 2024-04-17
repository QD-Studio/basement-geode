#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

#include "utils.hpp"

#ifdef GEODE_IS_WINDOWS
// Нам не нужен SteamAPI
bool initSteamAPI() {
    return 1;
}
#endif

$execute {
#ifdef GEODE_IS_WINDOWS
    // Убираем кнопку "Original" в описании уровня
    // Mod::get()->patch((void*)(base::get() + 0x1500AE), {0xE9, 0x01, 0x01, 0x00, 0x00, 0x90}); // Original button

    // Делаем текстурки в главном меню снежными, если сейчас зима
    if(basementutils::isWinterNow() && Mod::get()->getSettingValue<bool>("basement-resources")) {
        basementutils::patchString(base::get() + 0x27B4E8, "GJ_logo_002.png");
        basementutils::patchString(base::get() + 0x27B54D, "GJ_playBtn_002.png");
        basementutils::patchString(base::get() + 0x27B5D1, "GJ_garageBtn_002.png");
        basementutils::patchString(base::get() + 0x27B6C6, "GJ_creatorBtn_002.png");
        basementutils::patchString(base::get() + 0x27BC80, "GJ_moreGamesBtn_002.png");
    }

    // Меняем название окна
    basementutils::patchString(base::get() + 0x5B198, "Подвал ГДшеров");
    basementutils::patchString(base::get() + 0x5B224, "Подвал ГДшеров");

    // Хук SteamAPI чтобы не выдавались достижения Steam, которые были получены на приватном сервере
    Mod::get()->hook(
        (void*)(base::get() + 0x53020), &initSteamAPI, "initSteamAPI", tulip::hook::TulipConvention::Thiscall
    );
#elif defined(GEODE_IS_ANDROID)
    // Делаем текстурки в главном меню снежными, если сейчас зима
    if(basementutils::isWinterNow() && Mod::get()->getSettingValue<bool>("basement-resources")) {
        basementutils::patchString(0x2FD99E, 0x2FD99A, "GJ_logo_002.png");
        basementutils::patchString(0x2FD9EE, 0x2FD9EA, "GJ_playBtn_002.png");
        basementutils::patchString(0x2FDA54, 0x2FDA50, "GJ_garageBtn_002.png");
        basementutils::patchString(0x2FDB0A, 0x2FDB06, "GJ_creatorBtn_002.png");
        basementutils::patchString(0x2FE020, 0x2FE01C, "GJ_moreGamesBtn_002.png");
    }
#endif
}

// Избавляемся от синего фона
class $modify(CCSprite) {
    virtual bool initWithFile(char const* filename) {
        if(strcmp(filename, "GJ_gradientBG.png") == 0)
            this->setTag(3535);
        
        return CCSprite::initWithFile(filename);
    }

    virtual void setColor(cocos2d::_ccColor3B const& color) {
        if(this->getTag() == 3535)
            return;
        
        return CCSprite::setColor(color);
    }
};