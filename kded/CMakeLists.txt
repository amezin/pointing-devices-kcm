set(KDED_SRCS kded.h kded.cpp)

qt5_generate_dbus_interface(kded.h)
qt5_add_dbus_adaptor(KDED_SRCS "${CMAKE_CURRENT_BINARY_DIR}/kded.xml" kded.h PointingDevicesKDED)

add_library(kded_pointingdevices SHARED ${KDED_SRCS})
target_link_libraries(kded_pointingdevices
                      KF5::CoreAddons
                      KF5::ConfigCore
                      KF5::DBusAddons
                      pointingdevices)
install(TARGETS kded_pointingdevices DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kded_pointingdevices.desktop DESTINATION ${SERVICES_INSTALL_DIR}/kded)
