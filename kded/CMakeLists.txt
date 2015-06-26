find_package(KF5 REQUIRED COMPONENTS CoreAddons DBusAddons)

add_library(kded_pointingdevices SHARED kded.h kded.cpp)
target_link_libraries(kded_pointingdevices KF5::CoreAddons KF5::DBusAddons)
install(TARGETS kded_pointingdevices DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kded_pointingdevices.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kded)
