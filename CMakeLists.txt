cmake_minimum_required(VERSION 3.19)
project(TerraCan_MissionControl LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools SerialPort Location Quick Concurrent)

qt_standard_project_setup()

qt_add_executable(TerraCan_MissionControl
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tctelemetry.h
    # odbiornikcom.h odbiornikcom.cpp
    # odbiornikcom_worker.h
    # odbiornikcom_worker.cpp
    comhandler/odbiornikcom.cpp
    comhandler/odbiornikcom.h
    comhandler/odbiornikcom_worker.cpp
    comhandler/odbiornikcom_worker.h
    physics/physicsengine.cpp
    physics/physicsengine.h
    physics/physicshandler.cpp
    physics/physicshandler.h
    physics/physicsutils.h
    settingdialog.h settingdialog.cpp settingdialog.ui

)

target_include_directories(TerraCan_MissionControl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/physics
    ${CMAKE_CURRENT_SOURCE_DIR}/comhandler
    ${CMAKE_CURRENT_SOURCE_DIR}
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
        Qt6::Quick
        Qt6::Location
        Qt6::Concurrent
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
