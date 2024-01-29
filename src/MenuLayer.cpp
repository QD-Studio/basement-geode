#include "CreditsLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>

bool afkmode = false;
uint32_t iconsDestroyed = 0;

class $modify(AFKMode, MenuLayer) {
    bool shouldPlayAnimation = true;

    CCSprite* createBG(const char* spriteFilename) {
        auto sprite = CCSprite::create(spriteFilename);
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if(sprite) {
            float f = winSize.height / sprite->getContentSize().height;
            sprite->setScaleY(f);
            sprite->setScaleX(sprite->getScaleX() * f);
            sprite->setPositionX(winSize.width / 2);
            sprite->setPositionY(winSize.height / 2 + 45);
        }

        return sprite;
    }

    bool init() {
        if(!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        auto mainMenuBg = (CCNode*)this->getChildByID("main-menu-bg");
        mainMenuBg->setPositionY(-45);

        // Создаем бг
        auto menuBG = createBG("menubg.png"_spr);

        if(menuBG) {
            mainMenuBg->addChild(menuBG, 100);
            menuBG->setID("basement-bg");
        }

        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");

        // Выравниваем кнопки для бг
        if(Mod::get()->getSettingValue<bool>("basementResources")){
            auto playbtn = mainMenu->getChildByID("play-button");
            playbtn->setPositionY(-2);
            
            auto editorbtn = mainMenu->getChildByID("editor-button");
            editorbtn->setAnchorPoint({0.25f, 0.5f});
            editorbtn->setPosition({playbtn->getPositionX() + 82, -4});

            auto iconkit = mainMenu->getChildByID("icon-kit-button");
            iconkit->setAnchorPoint({0.75f, 0.5f});
            iconkit->setPosition({playbtn->getPositionX() - 82, -4});
        }

        // Кнопка кредитсов
        auto creditsSpr = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
        creditsSpr->setScale(1.325f);

        auto creditsBtn = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(CreditsLayer::switchToCustomLayerButton));
        creditsBtn->setPosition(mainMenu->convertToNodeSpace({winSize.width - 35, winSize.height - 27}));
        mainMenu->addChild(creditsBtn);

        // Это версия подвала
        CCLabelBMFont* text = CCLabelBMFont::create(fmt::format("BasementGDPS {}-Geode", basementutils::getVersion()).c_str(), "goldFont.fnt");
        text->setID("basement-version");
        this->addChild(text);
        text->setScale(0.5f);
        text->setPosition({winSize.width, 0});
        text->setAnchorPoint({1.0f, 0.0f});
        text->setZOrder(99);

        // Если сейчас зима, то начинаем снежную вечеринку
        if(isWinter) {
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
        this->scheduleOnce(SEL_SCHEDULE(&AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time"));

        return true;
    }

    void enterAFKMode(float) {
        afkmode = true;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto tempBG = createBG("menubg.png"_spr);

        if(tempBG) {
            auto mainMenuBg = (CCNode*)this->getChildByID("main-menu-bg");
            mainMenuBg->addChild(tempBG, -4);
            
            auto groundLayer = (CCNode*)mainMenuBg->getChildren()->lastObject();
            if(groundLayer)
                groundLayer->setZOrder(-10);
            
            auto menuBG = mainMenuBg->getChildByID("basement-bg");
            menuBG->setZOrder(-1);
            tempBG->setID("temp-bg");
            tempBG->runAction(CCSequence::create(CCDelayTime::create(1), CCFadeTo::create(1, 0), CCRemoveSelf::create(), NULL));
        } // Пришлось сделать так потому что кубикам и их частицам нельзя задать прозрачность


        auto title = (CCSprite*)this->getChildByID("main-title");
        title->runAction(CCFadeTo::create(1, 0));

        auto basementver = (CCLabelBMFont*)this->getChildByID("basement-version");
        basementver->runAction(CCFadeTo::create(1, 0));

        auto playerUsername = (CCNode*)this->getChildByID("player-username");
        playerUsername->runAction(CCFadeTo::create(1, 0));

        auto profileMenu = (CCMenu*)this->getChildByID("profile-menu");
        profileMenu->runAction(CCFadeTo::create(1, 0));
        profileMenu->setEnabled(false);

        auto iconsCounter = CCLabelBMFont::create(fmt::format("Icons destroyed: {}", iconsDestroyed).c_str(), "bigFont.fnt");
        iconsCounter->setPosition({winSize.width / 2, winSize.height - 30});
        iconsCounter->setOpacity(0);
        iconsCounter->setScale(0.5);
        iconsCounter->setID("icons-counter");
        this->addChild(iconsCounter);
        iconsCounter->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(1), CCFadeTo::create(1, 255)));

        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");
        mainMenu->runAction(CCFadeTo::create(1, 0));
        mainMenu->setEnabled(false);

        auto bottomMenu = (CCMenu*)this->getChildByID("bottom-menu");
        bottomMenu->runAction(CCFadeTo::create(1, 0));
        bottomMenu->setEnabled(false);

        auto topRightMenu = (CCMenu*)this->getChildByID("top-right-menu");
        topRightMenu->runAction(CCFadeTo::create(1, 0));
        topRightMenu->setEnabled(false);

        auto geodeIcon = bottomMenu->getChildByID("geode.loader/geode-button");
        auto fuckingGeodeIcon = (geode::CircleButtonSprite*)geodeIcon->getChildren()->objectAtIndex(0);
        auto fuckingGeodeSprite = (CCSprite*)fuckingGeodeIcon->getChildren()->objectAtIndex(0);
        fuckingGeodeSprite->runAction(CCFadeTo::create(1, 0));

        auto socialMediaMenu = (CCMenu*)this->getChildByID("social-media-menu");
        for(uint8_t i = 1; i < 6; i++) {
            auto btn = (CCNode*)socialMediaMenu->getChildren()->objectAtIndex(i);
            btn->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.25, 0), CCToggleVisibility::create()));
        }
    }

    void finishAFKModeClosing() {
        auto mainMenuBg = (CCNode*)this->getChildByID("main-menu-bg");
        if(mainMenuBg) {
            auto basementBg = (CCSprite*)mainMenuBg->getChildByID("basement-bg");
            basementBg->setZOrder(10);
            this->scheduleOnce(SEL_SCHEDULE(&AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time"));
        }
    }

    void exitAFKMode(CCObject* sender) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto iconsCounter = (CCLabelBMFont*)this->getChildByID("icons-counter");

        if(iconsCounter->numberOfRunningActions() != 0)
            return;

        afkmode = false;
        iconsDestroyed = 0;

        auto tempBG = createBG("menubg.png"_spr);
        if(tempBG) {
            auto mainMenuBg = (CCNode*)this->getChildByID("main-menu-bg");
            mainMenuBg->addChild(tempBG, 100);
            
            tempBG->setID("temp-bg");
            tempBG->setOpacity(0);
            tempBG->runAction(CCSequence::create(CCFadeTo::create(1, 255), CCCallFunc::create(this, SEL_CallFunc(&AFKMode::finishAFKModeClosing)),
                                                 CCRemoveSelf::create(), NULL));
        } // Пришлось сделать так потому что кубикам и их частицам нельзя задать прозрачность

        iconsCounter->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.5, 0), CCRemoveSelf::create()));

        auto title = (CCSprite*)this->getChildByID("main-title");
        title->runAction(CCFadeTo::create(1, 255));
        
        auto basementVersion = (CCLabelBMFont*)this->getChildByID("basement-version");
        basementVersion->runAction(CCFadeTo::create(1, 255));

        auto profileMenu = (CCMenu*)this->getChildByID("profile-menu");
        profileMenu->runAction(CCFadeTo::create(1, 255));
        profileMenu->setEnabled(true);

        auto playerUsername = (CCLabelBMFont*)this->getChildByID("player-username");
        playerUsername->runAction(CCFadeTo::create(1, 255));

        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");
        mainMenu->runAction(CCFadeTo::create(1, 255));
        mainMenu->setEnabled(true);

        auto bottomMenu = (CCMenu*)this->getChildByID("bottom-menu");
        bottomMenu->runAction(CCFadeTo::create(1, 255));
        bottomMenu->setEnabled(true);

        auto topRightMenu = (CCMenu*)this->getChildByID("top-right-menu");
        topRightMenu->runAction(CCFadeTo::create(1, 255));
        topRightMenu->setEnabled(true);

        auto geodeIcon = bottomMenu->getChildByID("geode.loader/geode-button");
        auto fuckingGeodeIcon = (geode::CircleButtonSprite*)geodeIcon->getChildren()->objectAtIndex(0);
        auto fuckingGeodeSprite = (CCSprite*)fuckingGeodeIcon->getChildren()->objectAtIndex(0);
        fuckingGeodeSprite->runAction(CCFadeTo::create(1, 255));

        auto socialMediaMenu = (CCMenu*)this->getChildByID("social-media-menu");
        for(uint8_t i = 1; i < 6; i++) {
            auto btn = (CCNode*)(socialMediaMenu)->getChildren()->objectAtIndex(i);
            btn->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.25, 255), CCToggleVisibility::create()));
        }
    }

    void onQuit(CCObject* sender) {
        if(afkmode) {
            return exitAFKMode(sender);
        }

        return MenuLayer::onQuit(sender);
    }
};

class $modify(MenuGameLayer) {
    void destroyPlayer() {
        if(afkmode) {
            iconsDestroyed++;
            auto text = (CCLabelBMFont*)GameManager::sharedState()->m_menuLayer->getChildByID("icons-counter");
            text->setString(fmt::format("Icons destroyed: {}", iconsDestroyed).c_str());
        }

        MenuGameLayer::destroyPlayer();
    }
};