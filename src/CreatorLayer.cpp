#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class $modify(Warning, CreatorLayer) {
    void showWarning() {
        auto alert = FLAlertLayer::create("Error", "Not available.", "OK");
        alert->show();
    }

    void onDailyLevel(CCObject*) {
        // auto dialogObj = DialogObject::create("The Keymaster", "we don't need it...", 2, 1.0f, false, {255, 255, 255});
        // auto dialog = DialogLayer::create(dialogObj, 2);
        // this->addChild(dialog);
        // dialog->animateInRandomSide();
        showWarning();
    }

    void onWeeklyLevel(CCObject*) {
        showWarning();
    }

    void onChallenge(CCObject*) {
        showWarning();  
    }
};