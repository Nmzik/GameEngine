add_compile_definitions(GLM_FORCE_XYZW_ONLY)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

file(GLOB SOURCES
*.h
*.cpp
)

if(WIN32)
	file(GLOB WINSOURCES
	windows/*.h
	windows/*.cpp
	)

	list (APPEND SOURCES ${WINSOURCES})


	file(GLOB OpenGL
	OpenGL/*.h
	OpenGL/*.cpp
	)
	
	#list (APPEND SOURCES ${OpenGL})
else()
	file(GLOB MACSOURCES
	macOS/*.h
	macOS/*.cpp
	)

	list (APPEND SOURCES ${MACSOURCES})
endif()

if(APPLE)
	add_executable(MyGameEngine MACOSX_BUNDLE ${SOURCES})
else()
	add_executable(MyGameEngine ${SOURCES})
endif()

foreach(_source IN ITEMS ${SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
#set_target_properties(MyGameEngine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/Release")

target_link_libraries(MyGameEngine BulletDynamics)
target_link_libraries(MyGameEngine BulletCollision)
target_link_libraries(MyGameEngine LinearMath)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_link_libraries(MyGameEngine opengl32.lib)
	target_link_libraries(MyGameEngine glew32.lib)
endif()
target_link_libraries(MyGameEngine zlibstatic)
target_link_libraries(MyGameEngine tinyxml2)
target_link_libraries(MyGameEngine Detour)
target_link_libraries(MyGameEngine Recast)
target_link_libraries(MyGameEngine freetype)
target_link_libraries(MyGameEngine tiny-aes)

#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyGameEngine)