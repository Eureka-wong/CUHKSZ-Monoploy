cmake_minimum_required(VERSION 3.16)

project(CUHKSZ_Tycoon VERSION 0.1 LANGUAGES CXX)

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
        Board.cpp
        Board.h
        Game.cpp
        Game.h
        Player.h
        Player.cpp
        Tile.cpp
        Tile.h
        BoardWidget.h     # 添加这行
        BoardWidget.cpp   # 添加这行
        strategyEngine.cpp
        action_weights.cpp
        strategyEngine.h
        action_weights.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CUHKSZ_Tycoon
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        CardDeck.h
        CardDeck.cpp
    )
else()
    if(ANDROID)
        add_library(CUHKSZ_Tycoon SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(CUHKSZ_Tycoon
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CUHKSZ_Tycoon PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
