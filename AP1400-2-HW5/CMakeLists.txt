cmake_minimum_required(VERSION 3.13)
project(AP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(ftxui REQUIRED)

include_directories(include/)

add_executable(main
        src/main.cpp
        src/unit_test.cpp
        src/espresso_based.cpp
        src/cappuccino.cpp
        src/mocha.cpp
)
target_link_libraries(main
        GTest::GTest
        GTest::Main
        ftxui::dom 
        ftxui::screen 
        ftxui::component
)