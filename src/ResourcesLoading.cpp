#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "utils.hpp"

using namespace geode::prelude;

// #if defined(GEODE_IS_WINDOWS)
// std::unordered_map<uintptr_t, const char*> resPatches = {
//     {0x273471, "CantLetGo.mp3"_spr},
//     {0x27352C, "Clubstep.mp3"_spr},
//     {0x27353D, "Electrodynamix.mp3"_spr},
//     {0x121782, "menuLoop.mp3"_spr},
// };
// #elif defined(GEODE_IS_ANDROID32)
// std::unordered_map<std::pair<uintptr_t, uintptr_t>, const char*> resPatches = {
//     {0x31D906, 0x31D902, "CantLetGo.mp3"_spr},
//     {0x31D948, 0x31D944, "Clubstep.mp3"_spr},
//     {0x31D94E, 0x31D94A, "Electrodynamix.mp3"_spr},
//     {0x3133CC, 0x3133C8, "menuLoop.mp3"_spr},
// };
// #endif

CCDictionary* addDict_hk(CCContentManager* self, const char* filename, bool idk) {
    if(std::string(filename) == "dragEffect.plist" && Mod::get()->getSettingValue<bool>("no-smoke")){
        return self->addDict(filename, idk);
    }

    if(std::filesystem::exists(Mod::get()->getResourcesDir() / filename)) {
        return self->addDict((Mod::get()->getResourcesDir() / filename).string().c_str(), idk);
    }
    
    return self->addDict(filename, idk);
}

// void patchMusic(bool value) {
//     for (auto& patch : resPatches) {
//         if (value) {
//             if (std::filesystem::exists(Mod::get()->getResourcesDir().parent_path() / patch.second)) {
// #if defined(GEODE_IS_WINDOWS)
//                 basementutils::patchString(base::get() + patch.first, patch.second);
// #elif defined(GEODE_IS_ANDROID32)
//                 basementutils::patchString(patch.first.first, patch.first.second, patch.second);      
// #endif
//             }
//         } else {
//             auto patches = Mod::get()->getPatches();
//             for (auto p : patches) {
// #if defined(GEODE_IS_WINDOWS)
//                 if (p->getAddress() == patch.first + base::get()) {
//                     Mod::get()->disownPatch(p);
//                 }
// #elif defined(GEODE_IS_ANDROID32)
//                 if (p->getAddress() == patch.first.first + base::get() || p->getAddress() == patch.first.second + base::get()) {
//                     Mod::get()->disownPatch(p);
//                 }
// #endif
//             }
//         }
// 	}
// }

$execute {
    // listenForSettingChanges("basement-music", +[](bool value) {
    //     // patchMusic(value);
    // });

    // patchMusic(Mod::get()->getSettingValue<bool>("basement-music"));

#if defined(GEODE_IS_WINDOWS)
    auto result = Mod::get()->hook(
        reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?addDict@CCContentManager@@QEAAPEAVCCDictionary@cocos2d@@PEBD_N@Z")), 
        &addDict_hk, "cocos2d::CCContentManager::addDict", tulip::hook::TulipConvention::Thiscall
    );
#elif defined(GEODE_IS_ANDROID)
    auto result = Mod::get()->hook(
        reinterpret_cast<void*>(dlsym(dlopen("libcocos2dcpp.so", RTLD_LAZY), "_ZN16CCContentManager7addDictEPKcb")), 
        &addDict_hk, "cocos2d::CCContentManager::addDict", tulip::hook::TulipConvention::Default
    );
#endif

//     if(result.isErr()) {
//         log::error("Can't hook {}. {}", result.value()->getDisplayName(), result.err());    
//     }
}

class $modify(CCTextureCache){
    CCTexture2D* addImage(char const* filename, bool idk){
        auto newName = Mod::get()->getResourcesDir() / filename;

        if (std::filesystem::exists(newName)) {
#ifdef GEODE_IS_WINDOWS
            return CCTextureCache::addImage(std::filesystem::relative(newName).string().c_str(), idk);
#else
            return CCTextureCache::addImage(basementutils::pathWithQuality(newName).c_str(), idk);
#endif
        }

        return CCTextureCache::addImage(filename, idk);
    }
};

class $modify(LoadingLayer){
    void loadAssets(){
        if(this->m_loadStep == 11 && Mod::get()->getSettingValue<bool>("basement-resources")){
            auto plist = Mod::get()->getResourcesDir() / "BasementOverrides.plist";
            auto png = Mod::get()->getResourcesDir() / "BasementOverrides.png";

            CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(basementutils::pathWithQuality(plist).c_str());

            // Я не понимаю почему функция не может загрузить .png автоматически, ведь в описании указано, что может
            // I don't understand why the function can't load .png automatically, the description states that it can
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(basementutils::pathWithQuality(plist).c_str(),
                CCTextureCache::sharedTextureCache()->addImage(basementutils::pathWithQuality(png).c_str(), false)
            );
        }

        LoadingLayer::loadAssets();
    }
};