#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "patches.hpp"
#include "DiscordButton.hpp"
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include "ModBadge.hpp"

using namespace geode::prelude;
using namespace keybinds;

$execute {
	log::info("BasementGDPS loading!");

    auto t = std::time(0);
    tm tm_;
    localtime_s(&tm_, &t);

    isWinter = tm_.tm_mon <= 1 || tm_.tm_mon == 11;
    
	initPatches();
	discordbutton::initPatches();

    BindManager::get()->registerBindable({"exitAFKmode"_spr, "Exit AFK mode", "", { 
            Keybind::create(KEY_Q, Modifier::None) 
        }, "Basement/MenuLayer/AFK Mode"
    });

    ModBadge::add(1, "modBadge_01_001.png");
    ModBadge::add(2, "modBadge_02_001.png");
    ModBadge::add(3, "modBadge_03_001.png");

    listenForSettingChanges("basementResources", +[](bool value) {
        basementutils::reloadAll();
    });    
}
