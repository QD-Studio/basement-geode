#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "utils.hpp"

using namespace geode::prelude;

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