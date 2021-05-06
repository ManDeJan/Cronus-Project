cmake_minimum_required(VERSION 3.15)

project(MyFirstPlugin VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-m32 -O3 -Wall -Wextra -Wpedantic)

file(GLOB_RECURSE SOURCES
	*.cc
)

include_directories(
    ../DynRPG/include
)

add_library(MyFirstPlugin SHARED ${SOURCES})
set_target_properties(MyFirstPlugin PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY
		${CMAKE_SOURCE_DIR}
)

