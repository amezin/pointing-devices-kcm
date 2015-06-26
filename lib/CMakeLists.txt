add_library(pointing-devices-common SHARED
            device.h
            device.cpp
            devicemanager.h
            devicemanager.cpp
            log.h
            log.cpp)
target_link_libraries(pointing-devices-common Qt5::Core)
target_include_directories(pointing-devices-common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

generate_export_header(pointing-devices-common)

add_subdirectory(x11)
add_subdirectory(test)

install(TARGETS pointing-devices-common ${INSTALL_TARGETS_DEFAULT_ARGS})
