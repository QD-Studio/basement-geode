#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/GJAccountSettingsLayer.hpp>

#include "utils.hpp"

using namespace geode::prelude;

class $modify(ProfilePage) {
    void onTwitter(CCObject* pObj) {
        CCApplication::sharedApplication()->openURL(std::string("https://discordapp.com/users/").assign(this->m_score->m_twitterURL).c_str());
    }
};

class $modify(GJAccountSettingsLayer) {
    bool init(int p1) {
        if(!GJAccountSettingsLayer::init(p1)) return false;
        
        auto children = this->m_mainLayer->getChildren();

        auto bg = (CCScale9Sprite*)(children->objectAtIndex(0));
        auto twitterText = (CCLabelBMFont*)(children->objectAtIndex(20));
        auto twitterInput = (CCTextInputNode*)(children->objectAtIndex(22));
        auto twitch1 = (CCNode*)(children->objectAtIndex(23));
        auto twitch2 = (CCNode*)(children->objectAtIndex(25));
        auto twitch3 = (CCNode*)(children->objectAtIndex(24));
        auto menu = (CCNode*)(children->objectAtIndex(1));
        auto update = (CCNode*)(menu->getChildren()->objectAtIndex(9));
        auto cancel = (CCNode*)(menu->getChildren()->objectAtIndex(8));

        twitterText->setString("Discord:");
        twitch1->setVisible(false);
        twitch2->setVisible(false);
        twitch3->setVisible(false);
        this->m_mainLayer->setPositionY(-21);
        bg->setPositionY(180);
        bg->setContentSize({400, 260});
        update->setPositionY(-87);
        cancel->setPositionY(-87);

        return true;
    }
};

$execute {
#ifdef GEODE_IS_WINDOWS
    basementutils::patchString(base::get() + 0x20927D, "discordapp.com/users/...");
    basementutils::patchString(base::get() + 0x2EEBD7, "gj_discordIcon_001.png");
    basementutils::patchString(base::get() + 0x2EEF39, "GJ_discordTxt_001.png");
#elif defined(GEODE_IS_ANDROID)
    // basementutils::patchString(0x340380, 0x3402F4, "discordapp.com/users/...");
    // basementutils::patchString(0x355FCC, 0x355DC0, "gj_discordIcon_001.png");
    // basementutils::patchString(0x355FEC, 0x355FB8, "GJ_discordTxt_001.png");
#endif
}