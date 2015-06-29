project(pointing-devices-kcm)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 REQUIRED COMPONENTS Core DBus Quick OPTIONAL_COMPONENTS Test)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Config DBusAddons Declarative I18n Package)

if (NOT Qt5Test_FOUND)
    set(BUILD_TESTING OFF CACHE BOOL "Build tests.")
endif()

include(GenerateExportHeader)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include(ECMMarkAsTest)

add_subdirectory(lib)
add_subdirectory(kded)
add_subdirectory(kcm)
