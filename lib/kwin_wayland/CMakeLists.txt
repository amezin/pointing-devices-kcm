find_package(KWinDBusInterface REQUIRED)

if(NOT KWIN_INPUT_DEVICEMANAGER_INTERFACE OR NOT KWIN_INPUT_DEVICE_INTERFACE)
    message(FATAL_ERROR "KWin is built without libinput dbus interface")
endif()

set(SRCS
    kwindevicemanager.h
    kwindevicemanager.cpp
    kwindevice.h
    kwindevice.cpp
    kwinproperty.h
    kwinproperty.cpp)

qt5_add_dbus_interfaces(SRCS
                        ${KWIN_INPUT_DEVICEMANAGER_INTERFACE}
                        ${KWIN_INPUT_DEVICE_INTERFACE})

add_library(pointingdevices_kwin MODULE ${SRCS})
target_link_libraries(pointingdevices_kwin
                      pointingdevices
                      Qt5::Core
                      Qt5::DBus)

install(TARGETS pointingdevices_kwin DESTINATION ${PLUGIN_INSTALL_DIR})
