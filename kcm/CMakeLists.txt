find_package(Qt5 REQUIRED COMPONENTS Quick)
find_package(KF5 REQUIRED COMPONENTS Declarative I18n CoreAddons Package)

add_library(pointing-devices-kcm MODULE kcm.h kcm.cpp)
target_link_libraries(pointing-devices-kcm
                      Qt5::Quick
                      KF5::CoreAddons
                      KF5::QuickAddons
                      KF5::I18n
                      pointing-devices-common)

install(TARGETS pointing-devices-kcm DESTINATION ${PLUGIN_INSTALL_DIR}/kcms)
install(FILES pointing-devices-kcm.desktop DESTINATION ${SERVICES_INSTALL_DIR})
kpackage_install_package(package pointing-devices-kcm kcms)

kcoreaddons_desktop_to_json(pointing-devices-kcm "pointing-devices-kcm.desktop")
