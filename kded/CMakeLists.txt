set(KDED_SRCS kded.h kded.cpp)

qt5_generate_dbus_interface(kded.h)
qt5_add_dbus_adaptor(KDED_SRCS "${CMAKE_CURRENT_BINARY_DIR}/kded.xml" kded.h PointingDevicesKDED)

add_library(kded_pointingdevices SHARED ${KDED_SRCS})
set_target_properties(kded_pointingdevices PROPERTIES
                      OUTPUT_NAME pointingdevices
                      PREFIX "")
target_link_libraries(kded_pointingdevices
                      KF5::CoreAddons
                      KF5::ConfigCore
                      KF5::DBusAddons
                      pointingdevices)
target_include_directories(kded_pointingdevices INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
generate_export_header(kded_pointingdevices)

install(TARGETS kded_pointingdevices DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kded)
kcoreaddons_desktop_to_json(kded_pointingdevices kded_pointingdevices.desktop)
