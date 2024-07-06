#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <gdlteam.gdl/api/api.hpp>

using namespace geode::prelude;

// Убиваем квесты, ежедневные и еженедельные уровни

class $modify(Warning, CreatorLayer) {
    void showWarning() {
        auto alert = FLAlertLayer::create("error"_gdl, "not-available"_gdl, "OK");
        alert->show();
    }

    void onDailyLevel(CCObject*) {
        showWarning();
    }

    void onWeeklyLevel(CCObject*) {
        showWarning();
    }

    void onChallenge(CCObject*) {
        showWarning();  
    }
};