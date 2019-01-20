cmake_minimum_required(VERSION 3.6.2)
project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(ECB=1)
add_compile_definitions(AES256=1)
add_compile_definitions(GLM_FORCE_XYZW_ONLY)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MSVC
set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_C_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
endif()

#Recast
set(RECASTNAVIGATION_DEMO OFF CACHE STRING "" FORCE)
set(RECASTNAVIGATION_EXAMPLES OFF CACHE STRING "" FORCE)
set(RECASTNAVIGATION_TESTS OFF CACHE STRING "" FORCE)

add_subdirectory(3rdparty/recastnavigation)

#Bullet
#add_definitions(=BT_NO_PROFILE=1)
set(BUILD_CLSOCKET OFF CACHE STRING "" FORCE)
set(BUILD_ENET OFF CACHE STRING "" FORCE)
set(BUILD_EXTRAS OFF CACHE STRING "" FORCE)
set(USE_GRAPHICAL_BENCHMARK OFF CACHE STRING "" FORCE)
set(USE_GLUT OFF CACHE STRING "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE STRING "" FORCE) # Don't statically link this
set(BUILD_BULLET2_DEMOS OFF CACHE STRING "" FORCE) # No samples
set(BUILD_OPENGL3_DEMOS OFF CACHE STRING "" FORCE) # No samples
set(BUILD_UNIT_TESTS OFF CACHE STRING "" FORCE) # No tests
set(BUILD_CPU_DEMOS OFF CACHE STRING "" FORCE) # No samples
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE STRING "" FORCE)
set(BUILD_BULLET3 OFF CACHE STRING "" FORCE) # Can use bullet2, bullet3 wants to build examples...

add_subdirectory(3rdparty/bullet)
add_subdirectory(3rdparty/zlib)
add_subdirectory(3rdparty/freetype)
add_subdirectory(3rdparty/sdl)

set(BUILD_STATIC_LIBS ON CACHE STRING "" FORCE)
set(BUILD_TESTING OFF CACHE STRING "" FORCE)
set(BUILD_TESTS OFF CACHE STRING "" FORCE)

add_subdirectory(3rdparty/tinyxml2)

include_directories(3rdparty/sdl/include)
include_directories(3rdparty/recastnavigation/Recast/Include)
include_directories(3rdparty/freetype/include)
include_directories(3rdparty/recastnavigation/Detour/Include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/3rdparty/zlib)
include_directories(3rdparty/zlib)
include_directories(3rdparty/glm)
include_directories(3rdparty/tinyxml2)
include_directories(3rdparty/bullet/src)

file(GLOB SOURCES
src/*.h
src/*.cpp
)

add_executable(MyGameEngine ${SOURCES})
#set_target_properties(MyGameEngine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/Release")

target_link_libraries(MyGameEngine SDL2)
target_link_libraries(MyGameEngine SDL2main)
target_link_libraries(MyGameEngine BulletDynamics)
target_link_libraries(MyGameEngine BulletCollision)
target_link_libraries(MyGameEngine LinearMath)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
target_link_libraries(MyGameEngine opengl32.lib)
endif()
target_link_libraries(MyGameEngine zlibstatic)
target_link_libraries(MyGameEngine tinyxml2)
target_link_libraries(MyGameEngine Detour)
target_link_libraries(MyGameEngine Recast)
target_link_libraries(MyGameEngine freetype)

#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyGameEngine)
