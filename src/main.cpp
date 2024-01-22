#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "patches.hpp"
// #ifdef GEODE_IS_WINDOWS
// #include <geode.custom-keybinds/include/Keybinds.hpp>
// #endif
#include <Geode/loader/SettingEvent.hpp>
#include "ModBadge.hpp"

using namespace geode::prelude;
// #ifdef GEODE_IS_WINDOWS
// using namespace keybinds;
// #endif

$execute {
	log::info("BasementGDPS loading!");

    auto t = std::time(0);
    tm tm_;
#ifdef GEODE_IS_WINDOWS
    localtime_s(&tm_, &t);
#elif defined(GEODE_IS_ANDROID)
    localtime_r(&t, &tm_);
#endif

    isWinter = tm_.tm_mon <= 1 || tm_.tm_mon == 11;
    

// #ifdef GEODE_IS_WINDOWS
//     BindManager::get()->registerBindable({"exitAFKmode"_spr, "Exit AFK mode", "", { 
//             Keybind::create(KEY_Q, Modifier::None) 
//         }, "Basement/MenuLayer/AFK Mode"
//     });
// #endif

    // ModBadge::add(1, "modBadge_01_001.png");
    // ModBadge::add(2, "modBadge_02_001.png");
    // ModBadge::add(3, "modBadge_03_001.png");
    // ModBadge::add(4, "modBadge_04_001.png");

    listenForSettingChanges("basementResources", +[](bool value) {
        basementutils::reloadAll();
    });
}
