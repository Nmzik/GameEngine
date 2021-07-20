	
add_executable(SPIRV_Converter main.cpp)

target_link_libraries(SPIRV_Converter spirv-cross-cpp spirv-cross-msl)
target_include_directories(SPIRV_Converter PUBLIC ${CMAKE_HOME_DIRECTORY}/3rdparty/SPIRV-Cross)