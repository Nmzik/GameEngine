#Recast
set(RECASTNAVIGATION_DEMO OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RECASTNAVIGATION_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(recastnavigation)

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
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)

add_subdirectory(bullet)

add_subdirectory(zlib)

add_subdirectory(freetype)

set(BUILD_STATIC_LIBS ON CACHE STRING "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(pugixml)

add_compile_definitions(ECB=1)
add_compile_definitions(AES256=1)

add_subdirectory(tiny-AES-c)
target_include_directories(tiny-aes INTERFACE tiny-AES-c)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/3rdparty/zlib)
include_directories(zlib)
