#include "patches.hpp"
// #include <Geode/modify/CCHttpRequest.hpp>
#include <Geode/modify/GManager.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

void setUrl_hk(cocos2d::extension::CCHttpRequest* self, char const* url){
    log::info("SETURL");
    std::string newURL = url;
        
    auto it = newURL.find("http://www.boomlings.com/database");
    if(it != std::string::npos){
        newURL.replace(it, 33, fmt::format("{}://www.podvalgdsherov.fun/pgcore", (Mod::get()->getSettingValue<bool>("http-encryption")) ? "https" : "http"));
    }

    return self->setUrl(newURL.c_str());
}

CCDictionary* addDict_hk(CCContentManager* self, const char* filename, bool idk) {
    if(std::string(filename) == "dragEffect.plist" && Mod::get()->getSettingValue<bool>("no-smoke")){
        return self->addDict(filename, idk);
    }

    if(ghc::filesystem::exists(Mod::get()->getResourcesDir() / filename)) {
        return self->addDict((Mod::get()->getResourcesDir() / filename).string().c_str(), idk);
    }
    
    return self->addDict(filename, idk);
}

void initPatches(){
    Mod::get()->patch((void*)(base::get() + 0x18B2B4), {0xB0, 0x01}); // Load Failed patch
    Mod::get()->patch((void*)(base::get() + 0x1500AE), {0xE9, 0x01, 0x01, 0x00, 0x00, 0x90}); // Original button
    
	for(auto& patch : trBG){ 
        Mod::get()->patch((void*)(base::get() + patch.address), patch.bytes);
    }

    if(isWinter && Mod::get()->getSettingValue<bool>("basementResources")) {
        // Да я ленивая ж и мне лень считать сколько будет конечный адрес
        basementutils::patchString(base::get() + 0x190856 + 1, "GJ_logo_002.png");
        basementutils::patchString(base::get() + 0x1908BB + 1, "GJ_playBtn_002.png");
        basementutils::patchString(base::get() + 0x19093F + 1, "GJ_garageBtn_002.png");
        basementutils::patchString(base::get() + 0x190A34 + 1, "GJ_creatorBtn_002.png");
        basementutils::patchString(base::get() + 0x190EF1 + 1, "GJ_moreGamesBtn_002.png");
    }

	for(auto& patch : resPatches){
		// if(ghc::filesystem::exists(Mod::get()->getResourcesDir() / patch.str)){
			basementutils::patchString(base::get() + patch.address + 1, patch.str);
		// }
	}

    basementutils::patchString(base::get() + 0x3CA28, "Подвал ГДшеров");
    basementutils::patchString(base::get() + 0x3CAA1, "Подвал ГДшеров");
    Mod::get()->patch((void*)(base::get() + 0x3CA9F), {28});


    Mod::get()->addHook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?addDict@CCContentManager@@QAEPAVCCDictionary@cocos2d@@PBD_N@Z")), 
        &addDict_hk, "cocos2d::CCContentManager::addDict", tulip::hook::TulipConvention::Thiscall
    );

    Mod::get()->addHook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libExtensions.dll"), "?setUrl@CCHttpRequest@extension@cocos2d@@QAEXPBD@Z")), 
        &setUrl_hk, "cocos2d::extension::CCHttpRequest::setUrl", tulip::hook::TulipConvention::Thiscall
    );
}

// class $modify(CCHttpRequest){
//     void setUrl(char const* url){
//         log::info("SETURL");
//         std::string newURL = url;
        
//         auto it = newURL.find("http://www.boomlings.com/database");
//         if(it != std::string::npos){
//             newURL.replace(it, 33, fmt::format("{}://www.podvalgdsherov.fun/pgcore", (Mod::get()->getSettingValue<bool>("http-encryption")) ? "https" : "http"));
//         }

//         return CCHttpRequest::setUrl(newURL.c_str());
//     }
// };

class $modify(GManager){
    void setup(){
        std::string basementFilename(this->m_fileName);
        basementFilename.replace(0, 2, "BS");

        this->m_fileName = basementFilename;

        GManager::setup();
    }
};

class $modify(GJBaseGameLayer) {
    void collectItem(int item, int count) {
        return GJBaseGameLayer::collectItem(std::clamp(item, 0, 1099), count);
    }

    void collectedObject(EffectGameObject* object) {
        if(object->m_targetGroupID != 0){
            object->m_targetGroupID = std::clamp(object->m_targetGroupID, 0, 1099);
        }

        return GJBaseGameLayer::collectedObject(object);
    }
};