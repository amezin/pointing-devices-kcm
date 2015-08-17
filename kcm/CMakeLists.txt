kcoreaddons_add_plugin(kcm_pointingdevices
                       SOURCES kcm.h
                               kcm.cpp
                               devicesettings.h
                               devicesettings.cpp
                               deviceproperty.h
                               deviceproperty.cpp
                       INSTALL_NAMESPACE "kcms")

target_link_libraries(kcm_pointingdevices
                      Qt5::Quick
                      KF5::CoreAddons
                      KF5::QuickAddons
                      KF5::ConfigCore
                      KF5::I18n
                      pointingdevices)

install(FILES kcm_pointingdevices.desktop DESTINATION ${SERVICES_INSTALL_DIR})
kpackage_install_package(package kcm_pointingdevices kcms)

kcoreaddons_desktop_to_json(kcm_pointingdevices "kcm_pointingdevices.desktop")
