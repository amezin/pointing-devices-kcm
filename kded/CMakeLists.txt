set(KDED_SRCS kded.h kded.cpp)

qt5_generate_dbus_interface(kded.h)
qt5_add_dbus_adaptor(KDED_SRCS "${CMAKE_CURRENT_BINARY_DIR}/kded.xml" kded.h PointingDevicesKDED)

kcoreaddons_add_plugin(kded_pointingdevices
                       SOURCES ${KDED_SRCS}
                       INSTALL_NAMESPACE "kf5/kded")
set_target_properties(kded_pointingdevices PROPERTIES
                      OUTPUT_NAME pointingdevices)
target_link_libraries(kded_pointingdevices
                      KF5::CoreAddons
                      KF5::ConfigCore
                      KF5::DBusAddons
                      pointingdevices)

kcoreaddons_desktop_to_json(kded_pointingdevices kded_pointingdevices.desktop)
