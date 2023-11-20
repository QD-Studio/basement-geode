#include "ModBadge.hpp"
#include "utils.hpp"

class $modify(CommentCell) {
/*
    cout << "modID: " << modID << endl;

    .text:0045FA0A ; 963:         if ( *(*(v2 + 392) + 340) != 2 )
    .text:0045FA0A                 mov     eax, [edi+188h]
    .text:0045FA10 ; 962:         v57 = "modBadge_02_001.png";
    .text:0045FA10                 mov     edx, offset aModbadge01001P ; "modBadge_01_001.png"
    .text:0045FA15                 mov     ecx, offset aModbadge02001P ; "modBadge_02_001.png"
    .text:0045FA1A                 cmp     dword ptr [eax+154h], 2
    .text:0045FA21 ; 964:           v57 = "modBadge_01_001.png";
    .text:0045FA21                 cmovnz  ecx, edx
    .text:0045FA24 ; 965:         v58 = cocos2d::CCSprite::createWithSpriteFrameName(v57);
    .text:0045FA24                 push    ecx
    .text:0045FA25                 call    ds:?createWithSpriteFrameName@CCSprite@cocos2d@@SAPAV12@PBD@Z ;
    cocos2d::CCSprite::createWithSpriteFrameName(char const *)

    patching out cmp ...
                cmovnz ...
    and mov ecx, ... to be the std::string from our map
*/
    void loadFromComment(GJComment* com) {
        auto modID = com->m_modBadge;
        if(!ModBadge::map.count(modID))
            return CommentCell::loadFromComment(com);

        // cout << "patching to " << map[modID].c_str() << endl;

        auto str = ModBadge::map[modID].c_str();

        Mod::get()->patch((void*)(base::get() + 0x5FA1A), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}); // cmp ...
        Mod::get()->patch((void*)(base::get() + 0x5FA21), {0x90, 0x90, 0x90});                         // cmovnz ...
        basementutils::patchString(base::get() + 0x5FA16, str);

        CommentCell::loadFromComment(com);
    }
};

class $modify(ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* score) {
        if(score->m_modBadge > 2) {
            auto alert = FLAlertLayer::create("Здесь неначто смотреть...", "Лучше не смотри в этот профиль.", "Понял");
            alert->show();
            return;
        }
        // log::error("{}", score->m_modBadge);
        // if(!ModBadge::map.count(score->m_modBadge))
        //     return ProfilePage::loadPageFromUserInfo(score);

        // log::info("patching to {}", ModBadge::map[score->m_modBadge]);

        // auto str = ModBadge::map[score->m_modBadge].c_str();

        // Mod::get()->patch((void*)(base::get() + 0x211180), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}); // cmp ...
        // Mod::get()->patch((void*)(base::get() + 0x211187), {0x90, 0x90, 0x90});                         // cmovnz ...
        // basementutils::patchString(base::get() + 0x21117C, str);

        ProfilePage::loadPageFromUserInfo(score);
    }
};

void ModBadge::add(int key, std::string value) {
    ModBadge::map.insert(std::make_pair(key, value));
}