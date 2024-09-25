#pragma once
#include <string>
#include <vector>
#include "utils.hpp"
#include <gdlteam.gdl/api/api.hpp>

struct Developer {
    std::string sprite;
    std::string name;
    std::string role;
};

class CreditsLayer : public CCLayer {
  protected:
    std::vector<Developer> devs = {
        {"kolyah35.png"_spr, "Kolyah35", "qd-studio-head"_gdl},
        {"bestrussiangdmodder.png"_spr, "Jaan", "lead-developer"_gdl},
        {"bestartmaker.png"_spr, "danilka22ah", "textures"_gdl},
        {"demolena.png"_spr, "Demolena", "marketing"_gdl}
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