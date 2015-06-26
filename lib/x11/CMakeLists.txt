find_package(XCB REQUIRED COMPONENTS XCB XINPUT EVENT)

add_library(pointingdevices_x11 SHARED
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
target_link_libraries(pointingdevices_x11
                      pointingdevices
                      Qt5::Core
                      ${XCB_XCB_LIBRARY} # TODO: What about imported targets?
                      ${XCB_XINPUT_LIBRARY}
                      ${XCB_EVENT_LIBRARY})
target_include_directories(pointingdevices_x11 PUBLIC
                           ${XCB_XCB_INCLUDE_DIR}
                           ${XCB_XINPUT_INCLUDE_DIR}
                           ${XCB_EVENT_INCLUDE_DIR})
generate_export_header(pointingdevices_x11)

add_subdirectory(test)

install(TARGETS pointingdevices_x11 DESTINATION ${PLUGIN_INSTALL_DIR})
