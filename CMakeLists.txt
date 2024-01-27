cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# function(setOption name value type)
#     set(${name} ${value} CACHE ${type} "" FORCE)
# endfunction()

# setOption(GEODE_BINDINGS_REPO_PATH STRING "${CMAKE_CURRENT_SOURCE_DIR}/bindings")
set(GEODE_BINDINGS_REPO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bindings)

# clang-win32.cmake
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
endif()

project(BasementGeode VERSION 1.6.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
add_subdirectory(external/utfcpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    "external/utfcpp/source"
)

setup_geode_mod(${PROJECT_NAME})
