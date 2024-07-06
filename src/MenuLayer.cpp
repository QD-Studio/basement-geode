#include "CreditsLayer.hpp"
#include "utils.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <gdlteam.gdl/api/api.hpp>

class $modify(AFKMode, MenuLayer) {
    struct Fields {
        bool m_afkmode = false;
        unsigned int m_iconsDestroyed = 0;
    };

    CCSprite* createBG() {
        auto sprite = CCSprite::create("menubg.png"_spr);
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if(sprite) {
            if (winSize.width > winSize.height)
                sprite->setScale(winSize.height / sprite->getContentSize().height);
            else
                sprite->setScale(winSize.width / sprite->getContentSize().width);

            sprite->setPosition(winSize / 2.0f);
        }

        return sprite;
    }

    CCSprite* createBG(const char* id) {
        auto ret = createBG();
        ret->setID(id);
        return ret;
    }

    bool init() {
        if (!MenuLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        constexpr auto startZ = 9999999;

        // afk mode setup
        auto mainMenuBg = (CCNode*)this->getChildByID("main-menu-bg");
        ((CCNode*)mainMenuBg->getChildren()->objectAtIndex(0))->setVisible(false); // original bg
        mainMenuBg->setZOrder(startZ + 1);
        mainMenuBg->setVisible(false);
        log::debug("test 0");

        addChild(createBG("basement-bg-1"), -99999); // bg under menu
        auto bg2 = createBG("basement-bg-2"); bg2->setOpacity(0); addChild(bg2, startZ); // bg under mgl (above menu)
        auto bg3 = createBG("basement-bg-3"); bg3->setOpacity(0); addChild(bg3, startZ + 2); // bg above mgl

        auto iconsCounter = CCLabelBMFont::create("Icons destroyed: 0", "bigFont.fnt");
        iconsCounter->setPosition({winSize.width / 2, winSize.height - 30});
        iconsCounter->setOpacity(0);
        iconsCounter->setScale(0.5);
        iconsCounter->setID("icons-counter");
        iconsCounter->setZOrder(startZ + 1);
        addChild(iconsCounter);

#ifndef GEODE_IS_ANDROID
        ((CCMenu*)getChildByID("close-menu"))->setZOrder(startZ + 3); // above everything
#endif

        // menu stuff
        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");

        // Выравниваем кнопки для бг
        if (Mod::get()->getSettingValue<bool>("basement-resources")) {
            auto bgBottomY = (bg2->getPositionY() - bg2->getScaledContentHeight() / 2.0f);
            auto posY = bgBottomY + 0.3f * bg2->getScaledContentHeight();
            auto localPosY = mainMenu->convertToNodeSpace({0.0f, posY}).y;
            
            auto playbtn = mainMenu->getChildByID("play-button");
            playbtn->setAnchorPoint({0.5f, 0.35f});
            playbtn->setPositionY(localPosY);
            
            auto editorbtn = mainMenu->getChildByID("editor-button");
            editorbtn->setAnchorPoint({0.25f, 0.35f});
            editorbtn->setPosition({playbtn->getPositionX() + 82, localPosY});

            auto iconkit = mainMenu->getChildByID("icon-kit-button");
            iconkit->setAnchorPoint({0.75f, 0.35f});
            iconkit->setPosition({playbtn->getPositionX() - 82, localPosY});
        }

        // Кнопка кредитсов
        auto creditsSpr = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
        creditsSpr->setScale(1.325f);

        auto creditsBtn = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(CreditsLayer::switchToCustomLayerButton));
        creditsBtn->setPosition(mainMenu->convertToNodeSpace({winSize.width - 35, winSize.height - 27}));
        mainMenu->addChild(creditsBtn);

        // Это версия подвала
        CCLabelBMFont* text = CCLabelBMFont::create(fmt::format("BasementGDPS {}-Geode", Mod::get()->getVersion()).c_str(), "goldFont.fnt");
        text->setID("basement-version");
        text->setScale(0.5f);
        text->setPosition({winSize.width / 2.0f, 0});
        text->setAnchorPoint({0.5f, 0.0f});
        text->setZOrder(99);
        addChild(text);

        auto titletext = getChildByID("main-title");
        titletext->setScale(1.2f);
        titletext->setPositionY(titletext->getPositionY() - 10);

        // Если сейчас зима, то начинаем снежную вечеринку
        if(basementutils::isWinterNow()) {
            CCParticleSnow* snow = CCParticleSnow::create();

            snow->setPosition({winSize.width / 2, winSize.height + 10});
            snow->setZOrder(100);
            snow->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.png"_spr, false));
            this->addChild(snow);

            auto title = (CCSprite*)this->getChildByID("main-title");
            title->setScale(0.75f);
            title->setPositionY(260);

            CCLayer* snowlayer = CCLayer::create();
            snowlayer->setID("basement-snow-layer");

            auto snow1 = CCSprite::createWithSpriteFrameName("snow_01.png"_spr);
            snow1->setPosition({ 508, 11 });
            snow1->setScale(0.175);

            auto snow2 = CCSprite::createWithSpriteFrameName("snow_01.png"_spr);
            snow2->setPosition({ 236, 0 });
            snow2->setScale(0.350);

            auto snow3 = CCSprite::createWithSpriteFrameName("snow_01.png"_spr);
            snow3->setPosition({ 80, 0 });
            snow3->setScale(0.350);

            auto snow4 = CCSprite::createWithSpriteFrameName("snow_01.png"_spr);
            snow4->setPosition({ 388, 3 });
            snow4->setScale(0.300);
            snow4->setZOrder(1);

            auto snoww1 = CCSprite::createWithSpriteFrameName("snow_02.png"_spr);
            snoww1->setPosition({325, 0});
            snoww1->setAnchorPoint({ 0.5, 0 });
            snoww1->setScale(0.175);

            snowlayer->addChild(snoww1);
            snowlayer->addChild(snow1);
            snowlayer->addChild(snow2);
            snowlayer->addChild(snow3);
            snowlayer->addChild(snow4);
            
            this->addChild(snowlayer);
        }

        // Входим в AFKMode
        this->scheduleOnce(schedule_selector(AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time"));

        return true;
    }

    void enterAFKMode(float) {
        m_fields->m_afkmode = true;

        getChildByID("basement-bg-2")->runAction(CCFadeIn::create(0.5f));
        getChildByID("basement-bg-3")->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeOut::create(0.5f), nullptr));
        getChildByID("main-menu-bg")->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCToggleVisibility::create(), nullptr));

        auto children = getChildren();
        for (auto i = 0; i < children->count(); i++) {
            auto node = (CCNode*)children->objectAtIndex(i);
            if (dynamic_cast<CCMenu*>(node) != nullptr) {
                ((CCMenu*)node)->setEnabled(false);
            }
        }

#ifndef GEODE_IS_ANDROID
        ((CCMenu*)getChildByID("close-menu"))->setEnabled(true);
#endif

        getChildByID("icons-counter")->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeIn::create(0.5f), nullptr));
    }

    void exitAFKMode(CCObject* sender) {
        m_fields->m_afkmode = false;

        getChildByID("basement-bg-3")->runAction(CCSequence::create(CCFadeIn::create(0.5f), CCFadeOut::create(0), nullptr));
        getChildByID("main-menu-bg")->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCToggleVisibility::create(), nullptr));
        getChildByID("basement-bg-2")->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCFadeOut::create(0.5f), nullptr));

        auto children = getChildren();
        for (auto i = 0; i < children->count(); i++) {
            auto node = (CCNode*)children->objectAtIndex(i);
            if (dynamic_cast<CCMenu*>(node) != nullptr) {
                ((CCMenu*)node)->setEnabled(true);
            }
        }

        getChildByID("icons-counter")->runAction(CCFadeOut::create(0.5f));

        this->scheduleOnce(schedule_selector(AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time") + 1); // + 1 because animation takes 1s
    }

    void onQuit(CCObject* sender) {
        if (m_fields->m_afkmode) {
            return exitAFKMode(sender);
        }

        return MenuLayer::onQuit(sender);
    }
};

class $modify(MenuGameLayer) {
    void destroyPlayer() {
        auto menulayer = (AFKMode*)GameManager::sharedState()->m_menuLayer;
        if (menulayer && menulayer->m_fields->m_afkmode) {
            auto text = (CCLabelBMFont*)menulayer->getChildByID("icons-counter");
            text->setString(fmt::format("{}: {}", "icons-destroyed"_gdl, ++menulayer->m_fields->m_iconsDestroyed).c_str());
        }

        MenuGameLayer::destroyPlayer();
    }
};