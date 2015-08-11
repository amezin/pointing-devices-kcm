add_library(pointingdevices SHARED
            device.h
            device.cpp
            devicemanager.h
            devicemanager.cpp
            log.h
            log.cpp)
target_link_libraries(pointingdevices Qt5::Core KF5::CoreAddons)
target_include_directories(pointingdevices INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

generate_export_header(pointingdevices)

option(BUILD_X11_BACKEND "Build X11/Xinput backend" ON)
if(BUILD_X11_BACKEND)
    add_subdirectory(x11)
endif()

option(BUILD_KWIN_BACKEND "Build KWin/Wayland backend" OFF)
if(BUILD_KWIN_BACKEND)
    add_subdirectory(kwin_wayland)
endif()
add_subdirectory(test)

install(TARGETS pointingdevices ${INSTALL_TARGETS_DEFAULT_ARGS})
