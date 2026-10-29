cmake_minimum_required(VERSION 3.19)
project(TerraCan_MissionControl LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort)

qt_standard_project_setup()

qt_add_executable(TerraCan_MissionControl
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tctelemetry.h
    odbiornikcom.h odbiornikcom.cpp
)

qt_add_translations(
    TARGETS TerraCan_MissionControl
    TS_FILES TerraCan_MissionControl_pl_PL.ts
)

target_link_libraries(TerraCan_MissionControl
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

install(TARGETS TerraCan_MissionControl
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TerraCan_MissionControl
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
