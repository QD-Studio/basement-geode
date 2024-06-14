#pragma once
#include <string>
#include <vector>
#include "utils.hpp"

struct Developer {
    std::string sprite;
    std::string nick;
    std::string role;
};

class CreditsLayer : public CCLayer {
  protected:
    std::vector<Developer> devs = {
        {"qdstudio.basementgdps/kolyah35.png", "Kolyah35", "QD Studio head"},
        {"qdstudio.basementgdps/bestrussiangdmodder.png", "Jaan", "Lead developer"},
        {"qdstudio.basementgdps/bestartmaker.png", "danilka22ah", "Textures"},
        {"qdstudio.basementgdps/demolena.png", "Demolena", "Marketing"}
    };

    std::vector<std::string> remixcredits = {"Dry Out by TheDJPony", "Cant let Go by RedForce Productions", "Clubstep by DJ-Nate",
                                             "Electrodynamix by Digital Liquid", "Deltarune OST - Basement by Palpe"};

    virtual bool init();
    virtual void keyBackClicked();
    void backButtonCallback(CCObject*);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

  public:
    ~CreditsLayer();
    static CreditsLayer* create();
    void switchToCustomLayerButton(CCObject*);
};