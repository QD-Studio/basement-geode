#include <Geode/Geode.hpp>
#include <Geode/modify/AccountLoginLayer.hpp>
#include <Geode/modify/GJAccountManager.hpp>

using namespace geode::prelude;

bool logging = false;

class $modify(AccountLoginLayer) {
    void onSubmit(CCObject* sender) {
        logging = true;

        AccountLoginLayer::onSubmit(sender);

        logging = false;
    }
};

class $modify(GJAccountManager) {
    static gd::string getShaPassword(gd::string password) {
        if(logging) {
            return password;
        }

        return GJAccountManager::getShaPassword(password);
    }
};