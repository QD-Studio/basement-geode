#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include "ModBadge.hpp"
#include "utils.hpp"
#include <gdlteam.gdl/api/api.hpp>

using namespace geode::prelude;

$execute {
#ifdef GEODE_IS_WINDOWS
    SetConsoleOutputCP(65001); // utf8
#endif

    listenForSettingChanges("basement-resources"_spr, +[](bool value) {
        basementutils::reloadAll();
    });

    gdl::addTranslationsFromFile(gdl::GDL_ENGLISH, Mod::get()->getResourcesDir() / "en.json");
    gdl::addTranslationsFromFile(gdl::GDL_RUSSIAN, Mod::get()->getResourcesDir() / "ru.json");
}
