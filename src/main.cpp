#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "patches.hpp"
#include <Geode/loader/SettingEvent.hpp>
#include "ModBadge.hpp"

using namespace geode::prelude;
$execute {
    SetConsoleOutputCP(65001); // utf8
    
	log::info("BasementGDPS loading!");

    auto t = std::time(0);
    tm tm_;
#ifdef GEODE_IS_WINDOWS
    localtime_s(&tm_, &t);
#elif defined(GEODE_IS_ANDROID)
    localtime_r(&t, &tm_);
#endif

    isWinter = tm_.tm_mon <= 1 || tm_.tm_mon == 11;
    
    ModBadge::add(1, "modBadge_01_001.png");
    ModBadge::add(2, "modBadge_02_001.png");
    ModBadge::add(3, "modBadge_03_001.png");
    ModBadge::add(4, "modBadge_04_001.png");

    listenForSettingChanges("basementResources", +[](bool value) {
        basementutils::reloadAll();
    });

    listenForSettingChanges("test-instance", +[](bool value) {
        basementutils::reloadAll();
        GameManager::sharedState()->setup();
    });
}
