cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

project(TwitchPlugin VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -Wall -Wextra -Wpedantic -Wl,--subsystem,windows)

file(GLOB_RECURSE SOURCES
	${CMAKE_CURRENT_LIST_DIR}/main.cc
)

# add_library(TwitchPlugin SHARED ${SOURCES})
add_executable(TwitchPlugin ${SOURCES})

target_include_directories(TwitchPlugin PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../DynRPG/include
    ${CMAKE_CURRENT_LIST_DIR}/../fmt/include
    ${CMAKE_CURRENT_LIST_DIR}/../cpp-httplib
    ${CMAKE_CURRENT_LIST_DIR}/../SFML/include
    ${CMAKE_CURRENT_LIST_DIR}/../GSL/include
)

file(GLOB SFML_LIBS ${CMAKE_SOURCE_DIR}/../SFML/build/**/*.dll)

target_link_libraries(TwitchPlugin PRIVATE 
    ${CMAKE_SOURCE_DIR}/../DynRPG/lib/libDynRPG.a
    ${CMAKE_SOURCE_DIR}/../fmt/build/bin/libfmt.dll
    ${SFML_LIBS}
    ws2_32
    crypt32
)

# add_custom_command(TARGET TwitchPlugin POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TwitchPlugin> ${CMAKE_SOURCE_DIR}/../Cronus/DynPlugins/TwitchPlugin.dll
#     COMMENT "Created ${CMAKE_SOURCE_DIR}/../Cronus/DynPlugins/TwitchPlugin.dll"
# )
