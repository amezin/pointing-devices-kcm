add_library(kcm_pointingdevices MODULE kcm.h kcm.cpp)
target_link_libraries(kcm_pointingdevices
                      Qt5::Quick
                      KF5::CoreAddons
                      KF5::QuickAddons
                      KF5::I18n
                      pointingdevices)

install(TARGETS kcm_pointingdevices DESTINATION ${PLUGIN_INSTALL_DIR}/kcms)
install(FILES kcm_pointingdevices.desktop DESTINATION ${SERVICES_INSTALL_DIR})
kpackage_install_package(package kcm_pointingdevices kcms)

kcoreaddons_desktop_to_json(kcm_pointingdevices "kcm_pointingdevices.desktop")
