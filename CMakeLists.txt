cmake_minimum_required(VERSION 3.16)

project(GameOfLifeDEMO VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        core/Cell.cpp
        core/Cell.h
        core/Rule.cpp
        core/Rule.h
        core/Grid.cpp
        core/Grid.h
        core/Simulator.cpp
        core/Simulator.h
        core/Pattern.cpp
        core/Pattern.h
        ui/GridView.cpp
        ui/GridView.h
        ui/RulePanel.cpp
        ui/RulePanel.h
        ui/StatsPanel.cpp
        ui/StatsPanel.h
        ui/SideBar.cpp
        ui/SideBar.h
        ui/PatternPanel.cpp
        ui/PatternPanel.h
        ui/dialogs/GridSettingsDialog.cpp
        ui/dialogs/GridSettingsDialog.h
        ui/dialogs/RuleEditDialog.cpp
        ui/dialogs/RuleEditDialog.h
        ui/dialogs/PatternSaveDialog.cpp
        ui/dialogs/PatternSaveDialog.h
        utils/PatternSerializer.cpp
        utils/PatternSerializer.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GameOfLifeDEMO
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(GameOfLifeDEMO SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(GameOfLifeDEMO
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(GameOfLifeDEMO PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_link_libraries(GameOfLifeDEMO PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(WIN32)
    set(DATA_DESTINATION "data")
else()
    set(DATA_DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/GameOfLifeDEMO")
endif()

install(FILES data/patterns.json DESTINATION ${DATA_DESTINATION})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GameOfLifeDEMO)
endif()
set_target_properties(GameOfLifeDEMO PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GameOfLifeDEMO
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GameOfLifeDEMO)
endif()
