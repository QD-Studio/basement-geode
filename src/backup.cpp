#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/modify/GJAccountManager.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

void saveWithNotification(){
    auto notification = Notification::create("Saving...", NotificationIcon::Loading, 0.0f);
    notification->show();

    CCApplication::sharedApplication()->trySaveGame();

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


bool backFromExit = false;
Notification* notification;

class $modify(MenuLayer) {
    void FLAlert_Clicked(FLAlertLayer* alert, bool b) {
        if(b && alert->getTag() == 0 && GJAccountManager::sharedState()->m_accountID != 0 && Mod::get()->getSettingValue<bool>("auto-backup")){
            backFromExit = true;
            notification = Notification::create("Saving account...", NotificationIcon::Loading, 0);
            notification->show();

            GJAccountManager::get()->backupAccount();
            
            return;
        }

        return MenuLayer::FLAlert_Clicked(alert, b);
    }
};

class $modify(GJAccountManager) {
    void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response) {
        if(!backFromExit)
            return GJAccountManager::onProcessHttpRequestCompleted(client, response);
        
        if(response->isSucceed()){
            log::warn("NOTIFI {}", notification == nullptr);
            notification->setIcon(NotificationIcon::Success);
            notification->setString("Saved account!");
        } else {
            notification->setIcon(NotificationIcon::Error);
            notification->setString("Can't save account!");
        }

        notification->setTime(1.0f);
        backFromExit = false;

        CCDirector::sharedDirector()->getActionManager()->addAction(
            CCSequence::create(
                CCDelayTime::create(0.25f),
                CCCallFunc::create(CCDirector::sharedDirector()->getRunningScene(), SEL_CallFunc(&MenuLayer::exitGame)),
                nullptr
            ), CCDirector::sharedDirector()->getRunningScene(), false
        );
    }
};