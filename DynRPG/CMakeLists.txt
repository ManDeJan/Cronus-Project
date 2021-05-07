cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

project(DynRPG VERSION 0.32)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O1 -Wl,--subsystem,windows -static -Wl,-static -static-libgcc -static-libstdc++)

file(GLOB_RECURSE SOURCES
	src/*.cpp
)

add_library(DynRPG STATIC ${SOURCES})
set_target_properties(DynRPG PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY
		${CMAKE_SOURCE_DIR}/lib
)

target_include_directories(DynRPG
	PRIVATE
		src/
		include/
)
