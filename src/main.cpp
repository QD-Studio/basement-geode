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

    listenForSettingChanges("basement-resources", +[](bool value) {
        basementutils::reloadAll();
    });
}
