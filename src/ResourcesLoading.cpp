#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "utils.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

inline std::unordered_map<uintptr_t, const char*> resPatches = {
    {0x273471, "CantLetGo.mp3"_spr},
    {0x27352C, "Clubstep.mp3"_spr},
    {0x27353D, "Electrodynamix.mp3"_spr},
    {0x121782, "menuLoop.mp3"_spr},
};

CCDictionary* addDict_hk(CCContentManager* self, const char* filename, bool idk) {
    if(std::string(filename) == "dragEffect.plist" && Mod::get()->getSettingValue<bool>("no-smoke")){
        return self->addDict(filename, idk);
    }

    if(ghc::filesystem::exists(Mod::get()->getResourcesDir() / filename)) {
        return self->addDict((Mod::get()->getResourcesDir() / filename).string().c_str(), idk);
    }
    
    return self->addDict(filename, idk);
}

void patchMusic(bool value) {
    for (auto& patch : resPatches) {
        // log::info("MUSIC: {}", value);
        if (value) {
            // log::debug("ehllo? {} {}", Mod::get()->getResourcesDir().parent_path(), patch.second);
            if (ghc::filesystem::exists(Mod::get()->getResourcesDir().parent_path() / patch.second)) {
                // log::debug("kekw");
                basementutils::patchString(base::get() + patch.first, patch.second);
            }
        } else {
            auto patches = Mod::get()->getPatches();
            for (auto p : patches) {
                if (p->getAddress() == patch.first + base::get()) {
                    // p->disable();
                    Mod::get()->disownPatch(p);
                }
            }
        }
	}
}

$execute {
    using namespace geode::prelude;
    listenForSettingChanges("basement-music", +[](bool value) {
        patchMusic(value);
    });

    patchMusic(Mod::get()->getSettingValue<bool>("basement-music"));

    Mod::get()->hook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?addDict@CCContentManager@@QAEPAVCCDictionary@cocos2d@@PBD_N@Z")), 
        &addDict_hk, "cocos2d::CCContentManager::addDict", tulip::hook::TulipConvention::Thiscall
    );
}

#endif

class $modify(CCTextureCache){
    CCTexture2D* addImage(char const* filename, bool idk){
        if(ghc::filesystem::exists(Mod::get()->getResourcesDir() / basementutils::getQualityString(filename))) {
            return CCTextureCache::addImage((Mod::get()->getResourcesDir() / basementutils::getQualityString(filename)).string().c_str(), idk);
        }

        return CCTextureCache::addImage(filename, idk);
    }
};

class $modify(LoadingLayer){
    void loadAssets(){
        CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA8888);

        if(this->m_loadStep == 10) {
            auto plist = (Mod::get()->getResourcesDir() / basementutils::getQualityString("BasementTextures.plist")).string();
            auto png = (Mod::get()->getResourcesDir() / basementutils::getQualityString("BasementTextures.png")).string();

            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist.c_str(),
                CCTextureCache::sharedTextureCache()->addImage(png.c_str(), false)
            );
        }

        if(this->m_loadStep == 11 && Mod::get()->getSettingValue<bool>("basementResources")){
            auto plist = (Mod::get()->getResourcesDir() / basementutils::getQualityString("BasementOverrides.plist")).string();
            auto png = (Mod::get()->getResourcesDir() / basementutils::getQualityString("BasementOverrides.png")).string();

            CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plist.c_str());

            // Я не понимаю почему функция не может загрузить .png автоматически, ведь в описании указано, что может
            // I don't understand why the function can't load .png automatically, the description states that it can
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist.c_str(),
                CCTextureCache::sharedTextureCache()->addImage(png.c_str(), false)
            );
        }

        LoadingLayer::loadAssets();
    }
};