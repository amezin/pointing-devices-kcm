add_executable(eventmonitor eventmonitor.cpp)
target_link_libraries(eventmonitor pointing-devices-x11)
ecm_mark_as_test(eventmonitor)

find_program(XEPHYR_COMMAND Xephyr)
configure_file(config.h.in config.h ESCAPE_QUOTES @ONLY)

add_executable(test_xinputdevicemanager test_xinputdevicemanager.cpp)
target_link_libraries(test_xinputdevicemanager pointing-devices-x11 Qt5::Test)
add_test(test_xinputdevicemanager test_xinputdevicemanager)
ecm_mark_as_test(test_xinputdevicemanager)

add_executable(test_xcbatomcache test_xcbatomcache.cpp)
target_link_libraries(test_xcbatomcache pointing-devices-x11 Qt5::Test)
add_test(test_xcbatomcache test_xcbatomcache)
ecm_mark_as_test(test_xcbatomcache)
