add_compile_definitions(GLM_FORCE_XYZW_ONLY)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

file(GLOB SOURCES
*.h
*.cpp
)

if(WIN32)
	find_package(GLEW REQUIRED)
	if (GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
	endif()

	file(GLOB WIN_SOURCES
	windows/*.h
	windows/*.cpp
	)

	list (APPEND SOURCES ${WIN_SOURCES})

	#file(GLOB OpenGL
	#OpenGL/*.h
	#OpenGL/*.cpp
	#)

	#list (APPEND SOURCES ${OpenGL})
elseif(APPLE)
	set (METAL 1)

	if(METAL)
		add_compile_definitions(GLM_FORCE_LEFT_HANDED)

		file(GLOB METAL_SOURCES
		metal/*.h
		metal/*.mm
		metal/*.cpp
		)

		list (APPEND SOURCES ${METAL_SOURCES})
	endif()

	list (APPEND MAC_SOURCES
		macOS/AppDelegate.h
		macOS/AppDelegate.m
		macOS/main.m
		macOS/Shaders.metal
		macOS/ViewController.h
		macOS/ViewController.mm
	)

	list (APPEND SOURCES ${MAC_SOURCES})
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
endif()
target_link_libraries(MyGameEngine zlibstatic)
target_link_libraries(MyGameEngine pugixml)
target_link_libraries(MyGameEngine Detour)
target_link_libraries(MyGameEngine Recast)
target_link_libraries(MyGameEngine freetype)
target_link_libraries(MyGameEngine tiny-aes)

#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyGameEngine)
