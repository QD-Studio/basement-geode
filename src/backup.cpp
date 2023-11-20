#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

void saveWithNotification(){
    auto notification = Notification::create("Saving...", NotificationIcon::Loading, 0.0f);
    notification->show();

    GameManager::sharedState()->save();

    notification->setString("Saved!");
    notification->setIcon(NotificationIcon::Success);
    notification->hide();
}

class $modify(LevelInfoLayer){
    void onPlay(CCObject* sender){
        saveWithNotification();
        return LevelInfoLayer::onPlay(sender);
    }
};

class $modify(EditLevelLayer){
    void onPlay(CCObject* sender){
        saveWithNotification();
        return EditLevelLayer::onPlay(sender);
    }
    
    void onEdit(CCObject* sender){
        saveWithNotification();
        return EditLevelLayer::onEdit(sender);
    }
};