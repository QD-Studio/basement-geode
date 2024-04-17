#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include "ModBadge.hpp"
#include "utils.hpp"

using namespace geode::prelude;

$execute {
#ifdef GEODE_IS_WINDOWS
    SetConsoleOutputCP(65001); // utf8
#endif    

	log::info("BasementGDPS loading!");
    
    // ModBadge::add(1, "modBadge_01_001.png");
    // ModBadge::add(2, "modBadge_02_001.png");
    // ModBadge::add(3, "modBadge_03_001.png");
    // ModBadge::add(4, "modBadge_04_001.png");

    listenForSettingChanges("basement-resources", +[](bool value) {
        basementutils::reloadAll();
    });
}
