#include "DiscordButton.hpp"

class $modify(ProfilePage) {
    // bool init(int p1, bool p2){
    //     if(!ProdilePage::init(p1, p2)) return false;
    //
    //     auto updateSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"); // for test
    //     DiscordButton::accountID = self->m_nAccountID;
    //     DiscordButton::page = self;
    //     auto updateBtn = gd::CCMenuItemSpriteExtra::create(updateSpr, self, SEL_MenuHandler(&Utils::loadPage));
    //     auto menu = CCMenu::create();
    //     menu->addChild(updateBtn);
    //     self->addChild(menu);
    //
    //     return true;
    // }

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

void discordbutton::initPatches() {
    basementutils::patchString(base::get() + 0x13BDBD, "discordapp.com/users/...");
    basementutils::patchString(base::get() + 0x210ABF, "gj_discordIcon_001.png");
    basementutils::patchString(base::get() + 0x210DB3, "GJ_discordTxt_001.png");
}