cmake_minimum_required(VERSION 3.6.2)
project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
add_definitions(-D_HAS_EXCEPTIONS=0)

if(WIN32)
	add_definitions(/Za)
endif(WIN32)

#set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)

IF(WIN32)
   set(SDL2_ROOT CACHE PATH "Path where to find SDL2")
   set(SDL2_ROOT "${SDL2_ROOT}")
   set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include;${SDL2_ROOT}/include/SDL2;")
   set(SDL2_LIBRARIES "${SDL2_ROOT}/lib/x64/SDL2.lib;${SDL2_ROOT}/lib/x64/SDL2main.lib;")
ELSE (APPLE)
   find_package(SDL2 REQUIRED)
   include_directories(${SDL2_INCLUDE_DIR})
ENDIF (WIN32)

#if(WIN32)
#set(SDL2_ROOT CACHE PATH "Path where to find SDL2")
#if(NOT SDL2_ROOT)
#message(FATAL_ERROR "Error: SDL2 not found")
#else(NOT SDL2_ROOT)
#set(SDL2_ROOT "${SDL2_ROOT}")
#set(SDL2_INCLUDE_DIR "${SDL2_ROOT}/include;${SDL2_ROOT}/include/SDL2;")
#set(SDL2_LIBRARIES "${SDL2_ROOT}/lib/x64/SDL2.lib;${SDL2_ROOT}/lib/x64/SDL2main.lib;")
#endif(NOT SDL2_ROOT)
#else(WIN32)
#endif(WIN32)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(MyGameEngine/recastnavigation/Recast/Include)
include_directories(MyGameEngine/freetype/include)
include_directories(MyGameEngine/recastnavigation/Detour/Include)
include_directories(MyGameEngine/zlib)
include_directories(MyGameEngine/glm)
include_directories(MyGameEngine/bullet)

link_directories(MyGameEngine/freetype/objs)
link_directories(MyGameEngine/zlib)
link_directories(MyGameEngine/bullet/lib)
#link_directories(MyGameEngine/sdl2/lib/x64)
link_directories(MyGameEngine/recastnavigation/lib)
link_directories(MyGameEngine/glew/lib/Release/x64)
link_directories(MyGameEngine/freetype/objs)

file(GLOB SOURCES
MyGameEngine/*.h
MyGameEngine/*.cpp
)

add_executable(MyGameEngine ${SOURCES})

target_link_libraries(MyGameEngine ${SDL2_LIBRARIES})
target_link_libraries(MyGameEngine BulletDynamics.lib)
target_link_libraries(MyGameEngine BulletCollision.lib)
target_link_libraries(MyGameEngine LinearMath.lib)
target_link_libraries(MyGameEngine opengl32.lib)
target_link_libraries(MyGameEngine zlibwapi.lib)
target_link_libraries(MyGameEngine Detour.lib)
target_link_libraries(MyGameEngine Recast.lib)
target_link_libraries(MyGameEngine freetype.lib)
