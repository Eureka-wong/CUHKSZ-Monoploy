cmake_minimum_required(VERSION 3.20)

project(CUHKSZ_Monopoly_Online VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebSockets)

# Organize ALL source files in one place
set(PROJECT_SOURCES
    main.cpp
    online_client.cpp
    online_mainwindow.cpp
    online_mainwindow.h
    
    # Game logic files
    Game.h
    Game.cpp
    Board.h
    Board.cpp
    BoardWidget.h
    BoardWidget.cpp
    Player.h
    Player.cpp
    CardDeck.h
    CardDeck.cpp
    Tile.h
    Tile.cpp
    StrategyEngine.h
    StrategyEngine.cpp
    action_weights.h
    action_weights.cpp
    
    # Resources
    resources.qrc
)

# Use modern Qt6 way to create executable
qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    include/monopoly/online_client.h
    include/monopoly/core_types.h
)

# Include directories (modern way)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Qt libraries (modern way)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::WebSockets
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_QT_WEBSOCKET)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Finalize Qt executable (required for Qt6)
qt_finalize_executable(${PROJECT_NAME})

