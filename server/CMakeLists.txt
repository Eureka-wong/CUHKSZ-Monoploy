cmake_minimum_required(VERSION 3.16)
project(MonopolyOnlineServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 检查是否有 uWebSockets（手动检查 vcpkg 安装路径）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../CUHKSZ-Monoploy/Multiplayer/vcpkg/installed/x64-windows")
    set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../CUHKSZ-Monoploy/Multiplayer/vcpkg/installed/x64-windows")
else()
    set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-windows")
endif()

# 检查 uWebSockets
if(EXISTS "${VCPKG_INSTALLED_DIR}/share/unofficial-uwebsockets")
    message(STATUS "Found uWebSockets, enabling WebSocket support")
    set(USE_UWEBSOCKETS ON)
else()
    message(FATAL_ERROR "uWebSockets not found! Please install uWebSockets via vcpkg.")
endif()

# 添加 include 目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VCPKG_INSTALLED_DIR}/include
)

# 添加编译定义
add_definitions(-DUSE_UWEBSOCKETS -DUWS_NO_ZLIB)

# 创建可执行文件
add_executable(monopoly_online_server main.cpp)

# 链接库
find_library(USOCKETS_LIB NAMES usockets PATHS "${VCPKG_INSTALLED_DIR}/lib" NO_DEFAULT_PATH)
if(USOCKETS_LIB)
    target_link_libraries(monopoly_online_server PRIVATE "${USOCKETS_LIB}")
endif()

find_library(LIBUV_LIB NAMES uv libuv PATHS "${VCPKG_INSTALLED_DIR}/lib" NO_DEFAULT_PATH)
if(LIBUV_LIB)
    target_link_libraries(monopoly_online_server PRIVATE "${LIBUV_LIB}")
endif()
