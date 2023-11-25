#pragma once
#include <vector>
#include <Geode/Geode.hpp>
#include "utils.hpp"

using namespace geode::prelude;

struct PatchBytes {
    uintptr_t address;
    std::vector<uint8_t> bytes;
};

struct PatchString {
    uintptr_t address;
    const char* str;
};

inline std::vector<PatchBytes> trBG = {
    {0x15A174, {0xFF}}, {0x15A175, {0xFF}}, {0x15A16F, {0xFF}}, {0x15A16D, {0x90, 0xB1}},
    {0x15891D, {0xFF}}, {0x15891E, {0xFF}}, {0x158917, {0xFF}}, {0x158915, {0x90, 0xB1}},
    {0x6F7FB,  {0xFF}}, {0x6F7FC,  {0xFF}}, {0x6F7F6,  {0xFF}}, {0x6F7F4,  {0x90, 0xB1}},
    {0x1979AD, {0xFF}}, {0x1979AE, {0xFF}}, {0x1979A7, {0xFF}}, {0x1979A5, {0x90, 0xB1}},
    {0x17DBC1, {0xFF}}, {0x17DBC2, {0xFF}}, {0x17DBBB, {0xFF}}, {0x17DBB9, {0x90, 0xB1}},
    {0x176032, {0xFF}}, {0x176033, {0xFF}}, {0x176036, {0xFF}}, {0x176034, {0x90, 0xB1}},
    {0x4DF7E,  {0xFF}}, {0x4DF7F,  {0xFF}}, {0x4DF78,  {0xFF}}, {0x4DF76,  {0x90, 0xB1}}
};

inline std::vector<PatchString> resPatches = {
    // {0xFDCB, "achievement_01.ogg"},
    // {0x24D38, "achievement_01.ogg"},
    {0x189FE0, "CantLetGo.mp3"_spr},
    {0x18A09B, "Clubstep.mp3"_spr},
    {0x18A0AC, "Electrodynamix.mp3"_spr},
    
    {0x2452F, "menuLoop.mp3"_spr},
    {0x24976, "menuLoop.mp3"_spr},
    {0x249A3, "menuLoop.mp3"_spr},
    {0xCE8A7, "menuLoop.mp3"_spr},
    {0x14BB1B, "menuLoop.mp3"_spr},
    {0x1583EB, "menuLoop.mp3"_spr},
    {0x18CEFB, "menuLoop.mp3"_spr},
    {0x1907EE, "menuLoop.mp3"_spr},
    {0x1DDF5B, "menuLoop.mp3"_spr},
    {0x20D9E1, "menuLoop.mp3"_spr},
    {0x21F988, "menuLoop.mp3"_spr},
    {0x22471A, "menuLoop.mp3"_spr},
    {0x22B307, "menuLoop.mp3"_spr},
};

void initPatches();