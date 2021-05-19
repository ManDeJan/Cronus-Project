cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

project(AlwaysHasBeen VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall -Wextra -Wpedantic -Wl,--subsystem,windows)

file(GLOB_RECURSE SOURCES
	main.cc
)

add_library(AlwaysHasBeen SHARED ${SOURCES})

include_directories(
    ../DynRPG/include
    ../fmt/include
)

file(GLOB SFML_LIBS ${CMAKE_SOURCE_DIR}/../SFML/build/**/*.dll)

target_link_libraries(AlwaysHasBeen
	PRIVATE ${CMAKE_SOURCE_DIR}/../DynRPG/lib/libDynRPG.a)

target_link_libraries(
    AlwaysHasBeen PRIVATE ${CMAKE_SOURCE_DIR}/../fmt/build/bin/libfmt.dll
)

target_link_libraries(
    AlwaysHasBeen PRIVATE sapi
)

target_link_libraries(AlwaysHasBeen
    PRIVATE ${SFML_LIBS})

add_custom_command(TARGET AlwaysHasBeen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:AlwaysHasBeen> ${CMAKE_SOURCE_DIR}/../Cronus/DynPlugins/AlwaysHasBeen.dll
    COMMENT "Created ${CMAKE_SOURCE_DIR}/../Cronus/DynPlugins/AlwaysHasBeen.dll"
)
