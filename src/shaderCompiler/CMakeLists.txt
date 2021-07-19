	
add_executable(ShaderCompiler main.cpp)

target_link_libraries(ShaderCompiler spirv-cross-cpp spirv-cross-msl)
target_include_directories(ShaderCompiler PUBLIC ${CMAKE_HOME_DIRECTORY}/3rdparty/SPIRV-Cross)