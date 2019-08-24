#for iOS!!!
cmake_minimum_required(VERSION 3.14)

project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if(WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-DNOMINMAX)
	add_definitions (/D "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")

	add_compile_options(
		/Zi
		/Zo
		/permissive-
		/MP
	)

	# /GS- - No stack buffer overflow checks
	add_compile_options("$<$<CONFIG:Release>:/GS->")
endif()

#Recast
set(RECASTNAVIGATION_DEMO OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/recastnavigation)

#Bullet
#add_definitions(=BT_NO_PROFILE=1)
set(USE_MSVC_DISABLE_RTTI OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_ENET OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)
set(USE_GLUT OFF CACHE BOOL "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE) # Don't statically link this
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE) # No samples
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE) # No samples
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE) # No tests
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE) # No samples
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE) # Can use bullet2, bullet3 wants to build examples...

add_subdirectory(3rdparty/bullet)
#include(3rdparty/bullet/BulletConfig.cmake)

add_subdirectory(3rdparty/zlib)

add_subdirectory(3rdparty/freetype)

set(BUILD_STATIC_LIBS ON CACHE STRING "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/pugixml)

add_compile_definitions(ECB=1)
add_compile_definitions(AES256=1)

add_subdirectory(3rdparty/tiny-AES-c)

include_directories(3rdparty/recastnavigation/Recast/Include)
include_directories(3rdparty/recastnavigation/Detour/Include)
include_directories(3rdparty/freetype/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/3rdparty/zlib)
include_directories(3rdparty/zlib)
include_directories(3rdparty/glm)
include_directories(3rdparty/pugixml/src)
include_directories(3rdparty/bullet/src)
include_directories(3rdparty/tiny-AES-c)

add_subdirectory(src)
