#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

using namespace geode::prelude;

class $modify(LevelSelectLayer) {
    bool init(int lvl){
        if(!LevelSelectLayer::init(lvl)) return false;

        auto sprite = (CCSprite*)this->getChildren()->objectAtIndex(0);
        sprite->setColor({ 255, 255, 255 });

        auto ground = (CCNode*)this->getChildren()->objectAtIndex(1);
        ground->setVisible(false);

        return true;
    }

    void scrollLayerMoved(cocos2d::CCPoint){
        return;
    }
};