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
        {"kolyah35.png", "Kolyah35", "Автор идеи, руководитель, разработчик"},
        {"bestrussiangdmodder.png", "Jaan", "Главный разработчик"},
        {"bestartmaker.png", "danilka22ah", "Художник"},
        {"demolena.png", "Demolena", "Рекламный менеджер"},
        // {"relative.png", "Relative", "Разработчик"},
        // {"grishaasd.png", "GrishaAsd", "Модератор"},
        // {"gunslinga.png", "KCQurGun21", "Модератор"}
    };

    std::vector<std::string> remixcredits = {"Dry Out by TheDJPony", "Cant let Go by RedForce Productions", "Clubstep by DJ-Nate",
                                             "Electrodynamix by Digital Liquid", "Deltarune OST - Basement by Palpe"};

    virtual bool init();
    virtual void keyBackClicked();
    void backButtonCallback(CCObject*);

  public:
    static CreditsLayer* create();
    void switchToCustomLayerButton(CCObject*);
};