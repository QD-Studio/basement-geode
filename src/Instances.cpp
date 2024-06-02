#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/GManager.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/AccountLoginLayer.hpp>
#include <Geode/modify/GJAccountManager.hpp>
#include "utils.hpp"

using namespace geode::prelude;

bool shouldSwitchInstance = false;
gd::string password;

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

class $modify(AccountLoginLayer) {
    void onSubmit(CCObject* sender) {
        password = m_passwordInput->getString();

        AccountLoginLayer::onSubmit(sender);
    }
};

// Меняем URL сервера в рантайме
void sendRequest_hk(CCHttpClient* self, CCHttpRequest* request) {
    std::string newURL = request->getUrl();
    std::string body(request->getRequestData(), request->getRequestDataSize());
        
    auto it = newURL.find("https://www.boomlings.com/database");
    if(it != std::string::npos){
        newURL.replace(it, 34, basementutils::getServerURL(true));
    }

    request->setUrl(newURL.c_str());
    
    log::info("curl {} -d \"{}\" -A \"\"", newURL, body);

    if(newURL.find("loginGJAccount.php") != std::string::npos) {
        auto shapassword = GJAccountManager::get()->getShaPassword(password); 
        auto it = body.find(shapassword);
        body.replace(it, shapassword.capacity() - 1, password.c_str());

        request->setRequestData(body.c_str(), body.size());
    }
    
    return self->send(request);
}

$execute {
    // Хук для подмены URL сервера в рантайме
#if defined(GEODE_IS_WINDOWS)
    Mod::get()->hook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?send@CCHttpClient@extension@cocos2d@@QAEXPAVCCHttpRequest@23@@Z")), 
        &sendRequest_hk, "cocos2d::extension::CCHttpClient::send", tulip::hook::TulipConvention::Thiscall
    );
#elif defined(GEODE_IS_ANDROID)
        Mod::get()->hook(
        reinterpret_cast<void*>(dlsym(dlopen("libcocos2dcpp.so", RLTD_LAZY), "_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE")), 
        &sendRequest_hk, "cocos2d::extension::CCHttpClient::send", tulip::hook::TulipConvention::Default
    );
#endif
    
    listenForSettingChanges("basement-server", +[](std::string value) {
        shouldSwitchInstance = true;
        GameManager::sharedState()->save();
        basementutils::reloadAll();
    });
}