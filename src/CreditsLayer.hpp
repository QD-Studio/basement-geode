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
        {"kolyah35.png"_spr, "Kolyah35", "QD Studio head"},
        {"bestrussiangdmodder.png"_spr, "Jaan", "Lead developer"},
        {"bestartmaker.png"_spr, "danilka22ah", "Textures"},
        {"demolena.png"_spr, "Demolena", "Marketing"}
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