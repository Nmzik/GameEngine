cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ProjectName)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue)

find_library(log-lib
        log)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
add_library(engine SHARED
        src/main/cpp/engine.cpp)

target_include_directories(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/glm/glm)

target_link_libraries(engine
        android
        native_app_glue
        ${log-lib}
        )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/GameEngine)
target_link_libraries(engine MyGameEngine)