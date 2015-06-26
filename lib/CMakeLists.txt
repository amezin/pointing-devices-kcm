add_library(pointingdevices SHARED
            device.h
            device.cpp
            devicemanager.h
            devicemanager.cpp
            log.h
            log.cpp)
target_link_libraries(pointingdevices Qt5::Core)
target_include_directories(pointingdevices INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

generate_export_header(pointingdevices)

add_subdirectory(x11)
add_subdirectory(test)

install(TARGETS pointingdevices ${INSTALL_TARGETS_DEFAULT_ARGS})
