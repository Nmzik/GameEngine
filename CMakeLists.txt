cmake_minimum_required(VERSION 3.6.2)
project(MyGameEngine)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CONFIGURATION_TYPES Debug Release)

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
link_directories(MyGameEngine/glew/lib/Release/x64)
link_directories(MyGameEngine/freetype/objs)

file(GLOB SOURCES
    MyGameEngine/*.h
    MyGameEngine/*.cpp
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
