add_definitions(-DPLUGIN_DIR="$<TARGET_FILE_DIR:pointingdevices_x11>")

add_executable(test_kded test_kded.cpp)
target_link_libraries(test_kded pointingdevices kded_pointingdevices Qt5::Test)
add_test(test_kded test_kded)
ecm_mark_as_test(test_kded)

add_executable(standalone standalone.cpp)
target_link_libraries(standalone pointingdevices kded_pointingdevices)
