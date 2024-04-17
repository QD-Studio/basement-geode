#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

// Убиваем квесты, ежедневные и еженедельные уровни

class $modify(Warning, CreatorLayer) {
    void showWarning() {
        auto alert = FLAlertLayer::create("Error", "Not available.", "OK");
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