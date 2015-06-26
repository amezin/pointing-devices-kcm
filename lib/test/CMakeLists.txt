add_definitions(-DPLUGIN_DIR="$<TARGET_FILE_DIR:pointingdevices_x11>")

add_executable(test_devicemanager test_devicemanager.cpp)
target_link_libraries(test_devicemanager pointingdevices Qt5::Test)
add_test(test_devicemanager test_devicemanager)
ecm_mark_as_test(test_devicemanager)
