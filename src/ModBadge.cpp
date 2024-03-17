#include "ModBadge.hpp"
#include "utils.hpp"

class $modify(CommentCell) {
    void loadFromComment(GJComment* com) {
        auto modID = com->m_modBadge;

        if(!ModBadge::map.count(modID))
            return CommentCell::loadFromComment(com);

        auto str = ModBadge::map[modID].c_str();
#ifdef GEODE_IS_WINDOWS
        // Mod::get()->patch((void*)(base::get() + 0x84AD2), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}); // cmp ...
        // Mod::get()->patch((void*)(base::get() + 0x84AD9), {0x90, 0x90, 0x90});                         // cmovnz ...
        // basementutils::patchString(base::get() + 0x84ACE, str);
#endif
        CommentCell::loadFromComment(com);
    }
};

class $modify(ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* score) {
        if(!ModBadge::map.count(score->m_modBadge))
            return ProfilePage::loadPageFromUserInfo(score);

        auto str = ModBadge::map[score->m_modBadge].c_str();

#ifdef GEODE_IS_WINDOWS
        // Mod::get()->patch((void*)(base::get() + 0x211180), {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}); // cmp ...
        // Mod::get()->patch((void*)(base::get() + 0x211187), {0x90, 0x90, 0x90});                         // cmovnz ...
        // basementutils::patchString(base::get() + 0x21117C, str);
#endif

        ProfilePage::loadPageFromUserInfo(score);
    }
};

void ModBadge::add(int key, std::string value) {
    ModBadge::map.insert(std::make_pair(key, value));
}