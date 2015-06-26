add_executable(eventmonitor eventmonitor.cpp)
target_link_libraries(eventmonitor pointingdevices_x11)
ecm_mark_as_test(eventmonitor)

# TODO: find-modules with PkgConfig?
find_program(XVFB_COMMAND Xvfb)
configure_file(config.h.in config.h ESCAPE_QUOTES @ONLY)

add_executable(test_xinputdevicemanager test_xinputdevicemanager.cpp)
target_link_libraries(test_xinputdevicemanager pointingdevices_x11 Qt5::Test)
add_test(test_xinputdevicemanager test_xinputdevicemanager)
ecm_mark_as_test(test_xinputdevicemanager)

add_executable(test_xcbatomcache test_xcbatomcache.cpp)
target_link_libraries(test_xcbatomcache pointingdevices_x11 Qt5::Test)
add_test(test_xcbatomcache test_xcbatomcache)
ecm_mark_as_test(test_xcbatomcache)
