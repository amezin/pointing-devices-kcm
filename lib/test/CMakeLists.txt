add_definitions(-DPLUGIN_DIR="$<TARGET_FILE_DIR:pointing-devices-x11>")

add_executable(test_devicemanager test_devicemanager.cpp)
target_link_libraries(test_devicemanager pointing-devices-common Qt5::Test)
add_test(test_devicemanager test_devicemanager)
ecm_mark_as_test(test_devicemanager)
