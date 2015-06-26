project(pointing-devices-kcm)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(QT_MIN_VERSION "5.4.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core OPTIONAL_COMPONENTS Test)
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
