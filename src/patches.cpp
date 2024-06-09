#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

#include "utils.hpp"

#include "string.h"

#include <Zydis/Zydis.h>

void* AllocatePageNearAddress(void* targetAddr) {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    const uint64_t PAGE_SIZE = sysInfo.dwPageSize;

    // log::debug("PAGE SIZE 0x{:X}", PAGE_SIZE);

    uint64_t startAddr = (uint64_t(targetAddr) & ~(PAGE_SIZE - 1)); // round down to nearest page boundary
    uint64_t minAddr = std::min(startAddr - 0x7FFFFF00, (uint64_t)sysInfo.lpMinimumApplicationAddress);
    uint64_t maxAddr = std::max(startAddr + 0x7FFFFF00, (uint64_t)sysInfo.lpMaximumApplicationAddress);

    uint64_t startPage = (startAddr - (startAddr % PAGE_SIZE));

    uint64_t pageOffset = 1;
    while (1) {
        uint64_t byteOffset = pageOffset * PAGE_SIZE;
        uint64_t highAddr = startPage + byteOffset;
        uint64_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

        bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

        if (highAddr < maxAddr) {
            void* outAddr = VirtualAlloc((void*)highAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (outAddr)
                return outAddr;
        }

        if (lowAddr > minAddr) {
            void* outAddr = VirtualAlloc((void*)lowAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (outAddr != nullptr)
                return outAddr;
        }

        pageOffset++;

        if (needsExit) {
            break;
        }
    }

    return nullptr;
}

void patchStrNew(void* srcAddr, const char* str) {
    // 1. allocate memory near src addr (trampoline)
    // 2. write the instructions to it
    // 3. write the `call` to the allocated memory instruction to the src addr
    // 4. fill with `nop`

    ZydisDisassembledInstruction instruction;
    if (ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, (uintptr_t)srcAddr, srcAddr, ZYDIS_MAX_INSTRUCTION_LENGTH, &instruction))) {
        if (instruction.info.opcode != 0x8d) {
            log::error("instruction is not lea!");
            return;
        }
    } else {
        log::error("Failed to decomp!");
        return;
    }

    
    static std::vector<const char*> strings;
    strings.push_back(_strdup(str)); // now we actually own the string

    // 1.

    auto newMem = AllocatePageNearAddress(srcAddr);
    if (newMem == nullptr) {
        log::error("BRO WHAT THE HELL O MY GOD THERE IS NO WAYYYYYYYYYYYYYYYYYY");
        return;
    }

    // 2.

    // mov r11, stringAddress
    {
        uint8_t bytes[10] = {0x49, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *(uintptr_t*)(bytes + 2) = (uintptr_t)strings.back();
        memcpy(newMem, bytes, sizeof(bytes));
    }

    int offset = 0;

    // lea <original register>, [r11]
    {
        ZydisEncoderRequest req;
        memset(&req, 0, sizeof(req));

        req.mnemonic = ZYDIS_MNEMONIC_LEA;
        req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
        req.operand_count = 2;
        req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
        req.operands[0].reg.value = instruction.operands[0].reg.value; // use original register

        req.operands[1].type = ZYDIS_OPERAND_TYPE_MEMORY;
        req.operands[1].mem.base = ZYDIS_REGISTER_R11;
        req.operands[1].mem.displacement = 0;
        req.operands[1].mem.size = 8;

        ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
        ZyanUSize encoded_length = sizeof(encoded_instruction);

        if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length))) {
            log::error("Failed to encode instruction!");
            return;
        }

        memcpy((uint8_t*)newMem + 10, encoded_instruction, encoded_length);
        offset = encoded_length + 10;
    }

    // ret
    {
        ((uint8_t*)newMem)[offset] = 0xc3;
    }

    // 3.

    uint8_t newSrcBytes[] = {
        0xe8, 0x00, 0x00, 0x00, 0x00 // call ... (to go to our trampoline, using `call` instead of `jmp` so we can get back with `ret`)
    };

    const auto relAddr = (uint64_t)newMem - ((uint64_t)srcAddr + sizeof(newSrcBytes));

    *(int32_t*)(newSrcBytes + 1) = (int32_t)relAddr;

    Mod::get()->patch(srcAddr, ByteVector(newSrcBytes, newSrcBytes + sizeof(newSrcBytes)));

    // 4.
    Mod::get()->patch((uint8_t*)srcAddr + 5, {0x90, 0x90}); // lea is 7 bytes, jmp is 5 bytes
}

$execute {
#ifdef GEODE_IS_WINDOWS
    // Убираем кнопку "Original" в описании уровня
    // Mod::get()->patch((void*)(base::get() + 0x1500AE), {0xE9, 0x01, 0x01, 0x00, 0x00, 0x90}); // Original button

    // Делаем текстурки в главном меню снежными, если сейчас зима
    // if(basementutils::isWinterNow() && Mod::get()->getSettingValue<bool>("basement-resources")) {
    //     patchStrNew((void*)(base::get() + 0x313315), "GJ_logo_002.png");
    //     patchStrNew((void*)(base::get() + 0x313395), "GJ_playBtn_002.png");
    //     patchStrNew((void*)(base::get() + 0x313413), "GJ_garageBtn_002.png");
    //     patchStrNew((void*)(base::get() + 0x313553), "GJ_creatorBtn_002.png");
    //     patchStrNew((void*)(base::get() + 0x313BDE), "GJ_moreGamesBtn_002.png");
    // }

    // Меняем название окна
    // basementutils::patchString(base::get() + 0x5B198, "Подвал ГДшеров");
    // basementutils::patchString(base::get() + 0x5B224, "Подвал ГДшеров");

    // Пропускаем инициализацию SteamAPI_Init, чтобы не выдавались достижения, полученные в подвале
    Mod::get()->patch((void*)(base::get() + 0x4A2C8B), {0xE9, 0x42, 0x00, 0x00, 0x00}); // jmp loc_1404A2D1A
#elif defined(GEODE_IS_ANDROID32)
    // Делаем текстурки в главном меню снежными, если сейчас зима
    // if(basementutils::isWinterNow() && Mod::get()->getSettingValue<bool>("basement-resources")) {
    //     basementutils::patchString(0x2FD99E, 0x2FD99A, "GJ_logo_002.png");
    //     basementutils::patchString(0x2FD9EE, 0x2FD9EA, "GJ_playBtn_002.png");
    //     basementutils::patchString(0x2FDA54, 0x2FDA50, "GJ_garageBtn_002.png");
    //     basementutils::patchString(0x2FDB0A, 0x2FDB06, "GJ_creatorBtn_002.png");
    //     basementutils::patchString(0x2FE020, 0x2FE01C, "GJ_moreGamesBtn_002.png");
    // }
#endif
}

// Избавляемся от синего фона
class $modify(CCSprite) {
    virtual bool initWithFile(char const* filename) {
        if(strcmp(filename, "GJ_gradientBG.png") == 0)
            this->setTag(3535);
        
        return CCSprite::initWithFile(filename);
    }

    virtual void setColor(cocos2d::_ccColor3B const& color) {
        if(this->getTag() == 3535)
            return;
        
        return CCSprite::setColor(color);
    }
};