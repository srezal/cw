cmake_minimum_required(VERSION 3.24)
project(cw C)

set(CMAKE_C_STANDARD 11)

add_executable(cw main.c Tools.c Tools.h Utils.c Utils.h Color.c Color.h Config.c Config.h Point.c Point.h GraphicsIO.c GraphicsIO.h GraphicsStructs.h GraphicsUtils.c IO.h IO.c ToolsManager.h ToolsManager.c)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(cw ${PNG_LIBRARY})

target_link_libraries(cw m)
