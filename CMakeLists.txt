cmake_minimum_required(VERSION 3.6.2)
project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
add_definitions(-D_HAS_EXCEPTIONS=0)

#set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")

include_directories(MyGameEngine/recastnavigation/Recast/Include)
include_directories(MyGameEngine/freetype/include)
include_directories(MyGameEngine/recastnavigation/Detour/Include)
include_directories(MyGameEngine/zlib)
include_directories(MyGameEngine/glm)
include_directories(MyGameEngine/sdl2/include)
include_directories(MyGameEngine/bullet)

link_directories(MyGameEngine/freetype/objs)
link_directories(MyGameEngine/zlib)
link_directories(MyGameEngine/bullet/lib)
link_directories(MyGameEngine/sdl2/lib/x64)
link_directories(MyGameEngine/recastnavigation/lib)
link_directories(MyGameEngine/glew/lib/Release/x64)
link_directories(MyGameEngine/freetype/objs)

file(GLOB SOURCES
    MyGameEngine/*.h
    MyGameEngine/*.cpp
    MyGameEngine/*.c
)

add_executable(MyGameEngine ${SOURCES})

target_link_libraries(MyGameEngine SDL2.lib)
target_link_libraries(MyGameEngine SDL2main.lib)
target_link_libraries(MyGameEngine BulletDynamics.lib)
target_link_libraries(MyGameEngine BulletCollision.lib)
target_link_libraries(MyGameEngine LinearMath.lib)
target_link_libraries(MyGameEngine opengl32.lib)
target_link_libraries(MyGameEngine zlibwapi.lib)
target_link_libraries(MyGameEngine Detour.lib)
target_link_libraries(MyGameEngine Recast.lib)
target_link_libraries(MyGameEngine freetype.lib)
