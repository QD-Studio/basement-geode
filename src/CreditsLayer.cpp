#include "CreditsLayer.hpp"

CreditsLayer* CreditsLayer::create() {
    auto ret = new CreditsLayer();
    if(ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool CreditsLayer::init() {
    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto size = backgroundSprite->getContentSize();

    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setColor({100, 100, 100});
    backgroundSprite->setZOrder(-1);
    addChild(backgroundSprite);

    auto button = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this, menu_selector(CreditsLayer::backButtonCallback));


    auto menu = CCMenu::create();
    menu->addChild(button);
    menu->setPosition({25, winSize.height - 25});
    addChild(menu);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 35, true);

    setKeypadEnabled(true);
    setTouchEnabled(true);
    setMouseEnabled(true);

    auto layer = CCLayer::create();
    layer->setID("credits-layer");

    auto logo = CCSprite::createWithSpriteFrameName("GJ_logo_001.png");
    logo->setPosition({winSize.width / 2, winSize.height + 50});
    layer->addChild(logo);

    auto textOriginalDev = CCLabelBMFont::create("Geometry Dash developer:", "bigFont.fnt");
    textOriginalDev->setPosition({winSize.width / 2, logo->getPositionY() - 80});
    layer->addChild(textOriginalDev);

    auto robtopLogo = CCSprite::createWithSpriteFrameName("robtoplogo_small.png");
    robtopLogo->setPosition({winSize.width / 2, textOriginalDev->getPositionY() - 35});
    layer->addChild(robtopLogo);

    auto textBasementDevs = CCLabelBMFont::create("BasementGDPS team:", "bigFont.fnt");
    textBasementDevs->setPosition({winSize.width / 2, robtopLogo->getPositionY() - 60});
    layer->addChild(textBasementDevs);

    auto qdstudioLogo = CCSprite::createWithSpriteFrameName("qdstudio.png"_spr);
    qdstudioLogo->setPosition({winSize.width / 2, textBasementDevs->getPositionY() - 40});
    layer->addChild(qdstudioLogo);

    auto qdstudio = CCLabelBMFont::create("QD Studio", "goldFont.fnt");
    qdstudio->setPosition({qdstudioLogo->getPositionX(), qdstudioLogo->getPositionY() - 30});
    layer->addChild(qdstudio);

    for(size_t i = 0; i < this->devs.size(); i++) {
        auto developer = CCSprite::createWithSpriteFrameName(this->devs[i].sprite.c_str());
        auto textDeveloper = CCLabelBMFont::create(this->devs[i].nick.c_str(), "goldFont.fnt");
        auto textDeveloper1 = CCLabelBMFont::create(this->devs[i].role.c_str(), "bigFont.fnt");
        developer->setPosition({(winSize.width / 2) - 100, qdstudioLogo->getPositionY() - 70 - (50 * i)});
        developer->setAnchorPoint({0.5f, 0.8f});

        textDeveloper->setPosition({developer->getPositionX() + 40, developer->getPositionY()});
        textDeveloper->setAnchorPoint({0.0f, 0.5f});

        textDeveloper1->setPosition({textDeveloper->getPositionX(), textDeveloper->getPositionY() - 20});
        textDeveloper1->setAnchorPoint({0.0f, 0.5f});
        textDeveloper1->setScale(0.5f);

        layer->addChild(developer);
        layer->addChild(textDeveloper);
        layer->addChild(textDeveloper1);

    }

    auto textRemixCredits = CCLabelBMFont::create("Authors of the song remixes:", "bigFont.fnt");
    textRemixCredits->setPosition({winSize.width / 2, qdstudioLogo->getPositionY() - 60 - (50 * this->devs.size()) - 60});
    layer->addChild(textRemixCredits);

    for(size_t i = 0; i < remixcredits.size(); i++) {
        auto label = CCLabelBMFont::create(remixcredits[i].c_str(), "goldFont.fnt");
        label->setPosition({winSize.width / 2, textRemixCredits->getPositionY() - 40 - (30 * i)});
        layer->addChild(label);
    }

    auto thankyou = CCLabelBMFont::create(
        "Special thanks to YOU for playing on\n our private server, we really appreciate it =)", "bigFont.fnt");
    thankyou->setPosition({winSize.width / 2, qdstudioLogo->getPositionY() - 60 - (50 * this->devs.size()) - (winSize.width / 2)});
    thankyou->setScale(0.4f);
    thankyou->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    addChild(thankyou);

    layer->setPositionY(-300);
    layer->setScale(0.5f);
    addChild(layer);

    CCAction* a1 = CCMoveTo::create(30, {layer->getPositionX(), 600});
    a1->setTag(1);

    CCAction* a2 = CCMoveTo::create(30, {winSize.width / 2, winSize.height / 2});
    a2->setTag(2);

    layer->runAction(a1);
    thankyou->runAction(a2);

    return true;
}

void CreditsLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void CreditsLayer::backButtonCallback(CCObject* object) {
    keyBackClicked();
}

void CreditsLayer::switchToCustomLayerButton(CCObject* object) {
    auto layer = CreditsLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);

    auto transition = CCTransitionFade::create(0.5f, scene);

    CCDirector::sharedDirector()->pushScene(transition);
}

bool CreditsLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCLayer* layer = (CCLayer*)this->getChildByIDRecursive("credits-layer");
    layer->getScheduler()->setTimeScale(2.0f);
    
    return true;
}

void CreditsLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCLayer* layer = (CCLayer*)this->getChildByIDRecursive("credits-layer");
    layer->getScheduler()->setTimeScale(1.0f);
}

CreditsLayer::~CreditsLayer() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer* layer = (CCLayer*)this->getChildByIDRecursive("credits-layer");
    layer->getScheduler()->setTimeScale(1.0f);
}