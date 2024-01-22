#include "CreditsLayer.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
// #ifdef GEODE_IS_WINDOWS
// #include <geode.custom-keybinds/include/Keybinds.hpp>
// #endif

struct MenuButtons {
    CCPoint point;
    float anchorPointX;
};

bool afkmode = false;
bool shouldPlayAnimation = true;
uint32_t iconsDestroyed = 0;

class $modify(AFKMode, MenuLayer) {
    bool backupFromExit = false;
    Notification* backupAccountNotifiation;

    bool init() {
        if(!MenuLayer::init()) return false;

        auto sprite = CCSprite::create("menubg.png"_spr);
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto title = (CCSprite*)this->getChildByID("main-title");
        auto mainmenu = (CCMenu*)this->getChildByID("main-menu");
        auto buttonsMenu = (CCMenu*)this->getChildByID("bottom-menu");
        auto socials = (CCMenu*)this->getChildByID("social-media-menu");
        auto menu = (CCMenu*)this->getChildByID("top-right-menu");
        auto name = (CCNode*)this->getChildByID("player-username");
        auto gdlver = (CCLabelBMFont*)this->getChildByID("gdl-version");

        auto gameLayer = (CCNode*)this->getChildByID("main-menu-bg");
        gameLayer->setPositionY(-45);
        auto ground = (CCNode*)gameLayer->getChildren()->objectAtIndex(1); // 1

        if(sprite != nullptr) {
            float f = winSize.height / sprite->getContentSize().height;
            sprite->setScaleY(f);
            sprite->setScaleX(sprite->getScaleX() * f);
            sprite->setPositionX(winSize.width / 2);
            sprite->setPositionY(winSize.height / 2 + 45);

            ground->setZOrder(-2);
            gameLayer->addChild(sprite, 100);
            sprite->setID("basement-bg");
        }

        if(Mod::get()->getSettingValue<bool>("basementResources")){
            auto playbtn = mainmenu->getChildByID("play-button");
            playbtn->setPositionY(-2);
            
            auto editorbtn = mainmenu->getChildByID("editor-button");
            editorbtn->setAnchorPoint({0.25f, 0.5f});
            editorbtn->setPosition({playbtn->getPositionX() + 82, -4});

            auto iconkit = mainmenu->getChildByID("icon-kit-button");
            iconkit->setAnchorPoint({0.75f, 0.5f});
            iconkit->setPosition({playbtn->getPositionX() - 82, -4});
        }

        auto creditsSpr = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
        creditsSpr->setScale(1.325f);
        auto creditsBtn = CCMenuItemSpriteExtra::create(creditsSpr, this, menu_selector(CreditsLayer::switchToCustomLayerButton));

        // auto creditsMenu = CCMenu::create(creditsBtn);
        creditsBtn->setPosition(mainmenu->convertToNodeSpace({winSize.width - 35, winSize.height - 27}));
        mainmenu->addChild(creditsBtn);

        CCLabelBMFont* text = CCLabelBMFont::create(fmt::format("BasementGDPS {}-Geode", basementutils::getVersion()).c_str(), "goldFont.fnt");
        text->setID("basement-version");
        this->addChild(text);
        text->setScale(0.5f);
        text->setPosition({winSize.width, 0});
        text->setAnchorPoint({1.0f, 0.0f});
        text->setZOrder(99);

        if(isWinter) {
            CCParticleSnow* snow = CCParticleSnow::create();

            snow->setPosition({winSize.width / 2, winSize.height + 10});
            // snow->setEmissionRate(100);
            snow->setZOrder(100);
            snow->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.png"_spr, false));
            this->addChild(snow);

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

        this->scheduleOnce(SEL_SCHEDULE(&AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time"));

// #ifdef GEODE_IS_WINDOWS
//         this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
//             if (afkmode && event->isDown()) AFKMode::exitAFKMode(this); 
            
//             return ListenerResult::Propagate;
//         }, "exitAFKmode"_spr);
// #endif
        return true;
    }

    void enterAFKMode(float) {
        afkmode = true;
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto title = (CCSprite*)this->getChildByID("main-title");
        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");
        auto buttonsMenu = (CCMenu*)this->getChildByID("bottom-menu");
        auto socials = (CCMenu*)this->getChildByID("social-media-menu");
        auto menu = (CCMenu*)this->getChildByID("top-right-menu");
        auto name = (CCNode*)this->getChildByID("player-username");
        auto gameLayer = (CCNode*)this->getChildByID("main-menu-bg");
        auto sprite = (CCSprite*)gameLayer->getChildByID("basement-bg");
        auto gdlver = (CCLabelBMFont*)this->getChildByID("gdl-version");
        auto basementver = (CCLabelBMFont*)this->getChildByID("basement-version");

        auto tempBG = CCSprite::create("menubg.png"_spr);
        if(tempBG) {
            float f = winSize.height / tempBG->getContentSize().height;
            tempBG->setScaleY(f);
            tempBG->setScaleX(tempBG->getScaleX() * f);
            tempBG->setPositionX(winSize.width / 2);
            tempBG->setPositionY(winSize.height / 2 + 45);
            gameLayer->addChild(tempBG, 10);
            sprite->setZOrder(-1);
            tempBG->setID("temp-bg");
            tempBG->runAction(CCSequence::create(CCDelayTime::create(1), CCFadeTo::create(1, 0), CCRemoveSelf::create(), NULL));
        } // Пришлось сделать так потому что кубикам и их частицам нельзя задать прозрачность

        title->runAction(CCFadeTo::create(1, 0));
        basementver->runAction(CCFadeTo::create(1, 0));
        gdlver->runAction(CCFadeTo::create(1, 0));
        name->runAction(CCFadeTo::create(1, 0));

// #ifdef GEODE_IS_WINDOWS
//         auto binds = keybinds::BindManager::get()->getBindsFor("exitAFKmode"_spr);        
//         std::string keys = binds[0]->toString();

//         if(binds.size() > 1){
//             for(auto& bind : binds){
//                 keys.assign(" + " + bind->toString());
//             }
//         }
        
//         auto exitText = CCLabelBMFont::create(std::string("(" + keys + ") чтобы выйти из AFK режима").c_str(), "bigFont.fnt");
//         exitText->setPosition({winSize.width - (exitText->getContentSize().width / 4) - 10, 10});
//         exitText->setOpacity(0);
//         exitText->setScale(0.5);
//         exitText->setID("exit-hint");
//         this->addChild(exitText);

//         exitText->runAction(CCFadeTo::create(.5, 128));
// #endif

        auto iconsCounter = CCLabelBMFont::create("Иконок уничтожено: 0", "bigFont.fnt");
        iconsCounter->setPosition({winSize.width / 2, winSize.height - 30});
        iconsCounter->setOpacity(0);
        iconsCounter->setScale(0.5);
        iconsCounter->setID("icons-counter");
        this->addChild(iconsCounter);

        mainMenu->runAction(CCFadeTo::create(1, 0));
        iconsCounter->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(1), CCFadeTo::create(1, 255)));
        buttonsMenu->runAction(CCFadeTo::create(1, 0));
        menu->runAction(CCFadeTo::create(1, 0));

        menu->setEnabled(false);
        mainMenu->setEnabled(false);
        buttonsMenu->setEnabled(false);

        for(uint8_t i = 1; i < 4; i++) {
            auto btn = (CCNode*)socials->getChildren()->objectAtIndex(i);
            btn->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.25, 0), CCToggleVisibility::create()));
        }
    }

    void finishAFKModeClosing() {
        auto gameLayer = this->getChildByID("main-menu-bg");

        if(gameLayer != nullptr) {
            auto sprite = (CCSprite*)gameLayer->getChildByID("basement-bg");
            sprite->setZOrder(10);
            this->scheduleOnce(SEL_SCHEDULE(&AFKMode::enterAFKMode), Mod::get()->getSettingValue<int64_t>("afk-time"));
        }
    }

    void exitAFKMode(CCObject* sender) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto title = (CCSprite*)this->getChildByID("main-title");
        auto mainMenu = (CCMenu*)this->getChildByID("main-menu");
        auto buttonsMenu = (CCMenu*)this->getChildByID("bottom-menu");
        auto socials = (CCMenu*)this->getChildByID("social-media-menu");
        auto menu = (CCMenu*)this->getChildByID("top-right-menu");
        auto name = (CCNode*)this->getChildByID("player-username");
        auto gameLayer = (CCNode*)this->getChildByID("main-menu-bg");
#ifdef GEODE_IS_WINDOWS
        auto exitText = (CCLabelBMFont*)this->getChildByID("exit-hint");
#endif
        auto iconsCounter = (CCLabelBMFont*)this->getChildByID("icons-counter");

        if(iconsCounter->numberOfRunningActions() != 0)
            return;

        afkmode = false;
        iconsDestroyed = 0;

        auto tempBG = CCSprite::create("menubg.png"_spr);
        if(tempBG) {
            float f = winSize.height / tempBG->getContentSize().height;
            tempBG->setScaleY(f);
            tempBG->setScaleX(tempBG->getScaleX() * f);
            tempBG->setPositionX(winSize.width / 2);
            tempBG->setPositionY(winSize.height / 2 + 45);
            tempBG->setOpacity(0);
            gameLayer->addChild(tempBG, 100);
            tempBG->setID("temp-bg");
            tempBG->runAction(CCSequence::create(CCFadeTo::create(1, 255), CCCallFunc::create(this, SEL_CallFunc(&AFKMode::finishAFKModeClosing)),
                                                 CCRemoveSelf::create(), NULL));
        } // Пришлось сделать так потому что кубикам и их частицам нельзя задать прозрачность

#ifdef GEODE_IS_WINDOWS
        exitText->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.5, 0), CCRemoveSelf::create()));
#endif
        iconsCounter->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.5, 0), CCRemoveSelf::create()));

        title->runAction(CCFadeTo::create(1, 255));
        ((CCLabelBMFont*)this->getChildByID("basement-version"))->runAction(CCFadeTo::create(1, 255));
        ((CCLabelBMFont*)this->getChildByID("player-username"))->runAction(CCFadeTo::create(1, 255));

        mainMenu->runAction(CCFadeTo::create(1, 255));
        buttonsMenu->runAction(CCFadeTo::create(1, 255));
        menu->runAction(CCFadeTo::create(1, 255));
        mainMenu->setEnabled(true);
        menu->setEnabled(true);
        buttonsMenu->setEnabled(true);

        for(uint8_t i = 1; i < 4; i++) {
            auto btn = (CCNode*)(socials)->getChildren()->objectAtIndex(i);
            btn->runAction(CCSequence::createWithTwoActions(CCFadeTo::create(.25, 255), CCToggleVisibility::create()));
        }
    }
};

class $modify(MenuGameLayer) {
    void destroyPlayer() {
        if(afkmode) {
            iconsDestroyed++;
            auto text = (CCLabelBMFont*)GameManager::sharedState()->m_menuLayer->getChildByID("icons-counter");
            text->setString(fmt::format("Иконок уничтожено: {}", iconsDestroyed).c_str());
        }

        MenuGameLayer::destroyPlayer();
    }
};