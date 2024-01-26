#include <Geode/Geode.hpp>
#include <Geode/modify/CCIMEDispatcher.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include "utils.hpp"
#include <utf8.h>

using namespace geode::prelude;

class $modify(CCIMEDispatcher){
    void dispatchInsertText(const char* text, int len, cocos2d::enumKeyCodes idk) {
        log::info("lol what {}", (int)idk);
        auto p1 = basementutils::cp1251_to_utf8(text);

        if (strlen(p1.c_str()) == 1)
            return CCIMEDispatcher::dispatchInsertText(p1.c_str(), 1, idk);

        int cp = p1[1] << 8 | p1[0];
        
        if (cp == 0x401 || (0x410 <= cp && cp <= 0x44F) || cp == 0x451) {
            char chars[5] = { '\0' };

            basementutils::getUnicodeChar(cp, chars);
            
            auto a = std::string(chars);

            return CCIMEDispatcher::dispatchInsertText(a.c_str(), 2, idk);
        } else {
            return CCIMEDispatcher::dispatchInsertText(p1.c_str(), utf8::internal::sequence_length(p1.begin()), idk);
        }

        CCIMEDispatcher::dispatchInsertText(p1.c_str(), len, idk);
    }
};

class $modify(CCTextInputNode) {
    bool onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int){
        return false;
    }
};