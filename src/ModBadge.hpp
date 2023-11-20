#pragma once
#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <unordered_map>
#include <string>

using namespace geode::prelude;

namespace ModBadge {
    void add(int key, std::string value);

    inline std::unordered_map<int, std::string> map;
} // namespace ModBadge