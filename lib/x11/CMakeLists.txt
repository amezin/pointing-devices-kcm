find_package(XCB REQUIRED COMPONENTS XCB XINPUT EVENT)

add_library(pointing-devices-x11 SHARED
            xcbobject.h
            xcbconnection.h
            xcbatomcache.h
            xcbatomcache.cpp
            xinputdevice.h
            xinputdevice.cpp
            xinputdeviceadapter.h
            xinputdeviceadapter.cpp
            xinputdevicemanager.h
            xinputdevicemanager.cpp
            xinputdevicemanageradapter.h
            xinputdevicemanageradapter.cpp)
target_link_libraries(pointing-devices-x11
                      pointing-devices-common
                      Qt5::Core
                      ${XCB_XCB_LIBRARY} # TODO: What about imported targets?
                      ${XCB_XINPUT_LIBRARY}
                      ${XCB_EVENT_LIBRARY})
target_include_directories(pointing-devices-x11 PUBLIC
                           ${XCB_XCB_INCLUDE_DIR}
                           ${XCB_XINPUT_INCLUDE_DIR}
                           ${XCB_EVENT_INCLUDE_DIR})
generate_export_header(pointing-devices-x11)

add_subdirectory(test)

install(TARGETS pointing-devices-x11 DESTINATION ${PLUGIN_INSTALL_DIR})
