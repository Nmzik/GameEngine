cmake_minimum_required(VERSION 3.6.2)
project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
#add_definitions(-D_HAS_EXCEPTIONS=0)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
#set(CMAKE_CXX_FLAGS_RELEASE "/MT")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/bin")

add_subdirectory(3rdparty/recastnavigation)
add_subdirectory(3rdparty/bullet)
add_subdirectory(3rdparty/zlib 3rdparty/zlib)
add_subdirectory(3rdparty/freetype)
add_subdirectory(3rdparty/sdl)

include_directories(3rdparty/sdl/include)
include_directories(3rdparty/recastnavigation/Recast/Include)
include_directories(3rdparty/freetype/include)
include_directories(3rdparty/recastnavigation/Detour/Include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/3rdparty/zlib)
include_directories(3rdparty/zlib)
include_directories(3rdparty/glm)
include_directories(3rdparty/bullet/src)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/Release)

file(GLOB SOURCES
MyGameEngine/*.h
MyGameEngine/*.cpp
)

add_executable(MyGameEngine ${SOURCES})

target_link_libraries(MyGameEngine SDL2.dll)
target_link_libraries(MyGameEngine SDL2main.dll)
target_link_libraries(MyGameEngine BulletDynamics.lib)
target_link_libraries(MyGameEngine BulletCollision.lib)
target_link_libraries(MyGameEngine LinearMath.lib)
target_link_libraries(MyGameEngine opengl32.lib)
target_link_libraries(MyGameEngine zlibstatic.lib)
target_link_libraries(MyGameEngine Detour.lib)
target_link_libraries(MyGameEngine Recast.lib)
target_link_libraries(MyGameEngine freetype.lib)
