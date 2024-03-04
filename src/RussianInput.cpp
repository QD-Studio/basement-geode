#include <Geode/Geode.hpp>
#include <Geode/modify/CCIMEDispatcher.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include "utils.hpp"
#include <utf8.h>

using namespace geode::prelude;

class $modify(CCIMEDispatcher){
    void dispatchInsertText(const char* text, int len, cocos2d::enumKeyCodes idk) {
        // log::info("DIT {} {} {}", text, len, (int)idk);
        
        // for (auto p = text; *p != 0x0; p++) {
        //     log::debug("0x{:X}", (int)*p);
        // }
        
        auto p1 = basementutils::cp1251_to_utf8(text);

        // log::debug("p1: {}", p1);

        if (strlen(p1.c_str()) == 1)
            return CCIMEDispatcher::dispatchInsertText(p1.c_str(), 1, idk);

        int cp = p1[1] << 8 | p1[0];

        // log::debug("cp 0x{:X}", cp);
        
        if (cp == 0x401 || (0x410 <= cp && cp <= 0x44F) || cp == 0x451) {
            char chars[5] = { '\0' };

            basementutils::getUnicodeChar(cp, chars);
            
            auto a = std::string(chars);

            // log::debug("a: {}", a);

            return CCIMEDispatcher::dispatchInsertText(a.c_str(), 2, idk);
        } else {
            // log::debug("lol");
            return CCIMEDispatcher::dispatchInsertText(p1.c_str(), utf8::internal::sequence_length(p1.begin()), idk);
        }

        // log::debug("omega");

        CCIMEDispatcher::dispatchInsertText(p1.c_str(), len, idk);
    }
};

class $modify(CCTextInputNode) {
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, char const* text, int nLen, cocos2d::enumKeyCodes keyCodes) {
        return false;
    }
};