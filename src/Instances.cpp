#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/GManager.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/CCHttpRequest.hpp>
#include "utils.hpp"

using namespace geode::prelude;

bool shouldSwitchInstance = false;

// При смене пространства нужно уничтожить GameManager чтобы игра прочитала другие сохранения
class $modify(LoadingLayer) {
    bool init(bool restart) {
        if(shouldSwitchInstance) {
            GameManager::sharedState()->~GameManager(); // Это важно
            GameManager::sharedState()->init();
            shouldSwitchInstance = false;
        }

        return LoadingLayer::init(restart);
    }
};

// Делаем раздельные сохранения
class $modify(GManager){
    void setup(){
        std::string basementFilename(this->m_fileName);
        basementFilename.replace(0, 2, "BS"); // Brawl Stars

        auto name = Mod::get()->getSettingValue<std::string>("basement-server");
        
        if( name == "test") {
            basementFilename.replace(0, 6, "BSTEST");
        } else if (name == "local") {
            basementFilename.replace(0, 7, "BSLOCAL");
        }

        this->m_fileName = basementFilename;

        GManager::setup();
    }
};

// Убираем никому не нужные подсказки
class $modify(GameManager) {
    void firstLoad() {
        this->m_clickedEditor = true;
        this->m_clickedGarage = true;
        this->m_clickedPractice = true;   
    }
};

// Меняем URL сервера в рантайме
#if defined(GEODE_IS_WINDOWS)
void setUrl_hk(cocos2d::extension::CCHttpRequest* self, char const* url){
    std::string newURL = url;
        
    auto it = newURL.find("https://www.boomlings.com/database");
    if(it != std::string::npos){
        newURL.replace(it, 34, basementutils::getServerURL(true));
    }

    return self->setUrl(newURL.c_str());
}
#elif defined(GEODE_IS_ANDROID)
void sendRequest_hk(CCHttpClient* self, CCHttpRequest* request) {
    std::string newURL = request->getUrl();
        
    auto it = newURL.find("https://www.boomlings.com/database");
    if(it != std::string::npos){
        newURL.replace(it, 34, basementutils::getServerURL(true));
    }

    request->setUrl(newURL.c_str());
    return self->send(self, request);
}
#endif

$execute {
    // Хук для подмены URL сервера в рантайме
#if defined(GEODE_IS_WINDOWS)
    Mod::get()->hook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?setUrl@CCHttpRequest@extension@cocos2d@@QAEXPBD@Z")), 
        &setUrl_hk, "cocos2d::extension::CCHttpRequest::setUrl", tulip::hook::TulipConvention::Thiscall
    );
#elif defined(GEODE_IS_ANDROID)
        Mod::get()->hook(
        reinterpret_cast<void*>(dlsym(dlopen("libcocos2dcpp.so", RLTD_LAZY), "_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE")), 
        &setUrl_hk, "cocos2d::extension::CCHttpClient::send", tulip::hook::TulipConvention::Default
    );
#endif
    
    listenForSettingChanges("basement-server", +[](std::string value) {
        shouldSwitchInstance = true;
        GameManager::sharedState()->save();
        basementutils::reloadAll();
    });
}