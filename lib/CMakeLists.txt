add_library(pointing-devices-common SHARED log.h log.cpp)
target_link_libraries(pointing-devices-common Qt5::Core)
target_include_directories(pointing-devices-common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

generate_export_header(pointing-devices-common)

add_subdirectory(x11)
