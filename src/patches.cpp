#include "patches.hpp"
#include <Geode/modify/GManager.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCApplication.hpp>
#include <Geode/modify/AchievementManager.hpp>
#include <Geode/modify/GameManager.hpp>

void setUrl_hk(cocos2d::extension::CCHttpRequest* self, char const* url){
    std::string newURL = url;
        
    auto it = newURL.find("https://www.boomlings.com/database");
    if(it != std::string::npos){
        newURL.replace(it, 34, basementutils::getServerURL(true));
    }

    return self->setUrl(newURL.c_str());
}

bool initSteamAPI() {
    return 1;
}

$execute {
#ifdef GEODE_IS_WINDOWS
    // Mod::get()->patch((void*)(base::get() + 0x1500AE), {0xE9, 0x01, 0x01, 0x00, 0x00, 0x90}); // Original button
    
	// for(auto& patch : trBG){ 
    //     Mod::get()->patch((void*)(base::get() + patch.address), patch.bytes);
    // }

    if(isWinter && Mod::get()->getSettingValue<bool>("basementResources")) {
        basementutils::patchString(base::get() + 0x27B4E8, "GJ_logo_002.png");
        basementutils::patchString(base::get() + 0x27B54D, "GJ_playBtn_002.png");
        basementutils::patchString(base::get() + 0x27B5D1, "GJ_garageBtn_002.png");
        basementutils::patchString(base::get() + 0x27B6C6, "GJ_creatorBtn_002.png");
        basementutils::patchString(base::get() + 0x27BC80, "GJ_moreGamesBtn_002.png");
    }

    basementutils::patchString(base::get() + 0x5B198, "Подвал ГДшеров");
    basementutils::patchString(base::get() + 0x5B224, "Подвал ГДшеров");

    Mod::get()->hook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?setUrl@CCHttpRequest@extension@cocos2d@@QAEXPBD@Z")), 
        &setUrl_hk, "cocos2d::extension::CCHttpRequest::setUrl", tulip::hook::TulipConvention::Thiscall
    );

    Mod::get()->hook(
        (void*)(base::get() + 0x53020), &initSteamAPI, "initSteamAPI", tulip::hook::TulipConvention::Thiscall
    );
#endif
}

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

// class $modify(GJBaseGameLayer) {
//     void collectItem(int item, int count) {
//         return GJBaseGameLayer::collectItem(std::clamp(item, 0, 1099), count);
//     }

//     void collectedObject(EffectGameObject* object) {
//         if(object->m_targetGroupID != 0){
//             object->m_targetGroupID = std::clamp(object->m_targetGroupID, 0, 1099);
//         }

//         return GJBaseGameLayer::collectedObject(object);
//     }
// };