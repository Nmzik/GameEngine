if (CMAKE_VERSION GREATER_EQUAL 3.12.0)
	add_compile_definitions(GLM_FORCE_XYZW_ONLY)
	add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO_ONE)
	add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
else()
	add_definitions(-DGLM_FORCE_XYZW_ONLY)
	add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
	add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
endif()

file(GLOB SOURCES
	*.h
	*.cpp
)

file(GLOB LOADERS
	loaders/*.h
	loaders/*.cpp
)
list (APPEND SOURCES ${LOADERS})

file(GLOB UTILS
	utils/*.h
)
list (APPEND SOURCES ${UTILS})

file(GLOB ALLOCATORS
	allocators/*.h
	allocators/*.cpp
)
list (APPEND SOURCES ${ALLOCATORS})

if(WIN32 OR ANDROID)
	file(GLOB Vulkan
		vulkan/*.h
		vulkan/*.cpp
	)
	list (APPEND SOURCES ${Vulkan})
endif()

if(ANDROID)
	file(GLOB ANDROID_SOURCES
		android/*.h
		android/*.cpp
	)
	list (APPEND SOURCES ${ANDROID_SOURCES})
endif()

if(WIN32)
	file(GLOB WIN_SOURCES
		windows/*.h
		windows/*.cpp
	)

	list (APPEND SOURCES ${WIN_SOURCES})

	file(GLOB OpenGL
		OpenGL/*.h
		OpenGL/*.cpp
	)
	list (APPEND SOURCES ${OpenGL})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL iOS)
	set (METAL 1)

	list (APPEND SOURCES
		ios/AppDelegate.h
		ios/AppDelegate.m
		ios/main.m
		ios/ViewController.h
		ios/ViewController.mm
		ios/LaunchScreen.storyboard
	)
elseif(APPLE)
	set (METAL 1)

	list (APPEND SOURCES
		macOS/AppDelegate.h
		macOS/AppDelegate.m
		macOS/main.m
		macOS/ViewController.h
		macOS/ViewController.mm
	)
endif()

if(METAL)
	add_compile_definitions(GLM_FORCE_LEFT_HANDED)

	list (APPEND METAL_SOURCES
		metal/MetalRenderer.h
		metal/MetalRenderer.mm
		metal/DebugDrawer.h
		metal/DebugDrawer.cpp
		metal/TextureDecompressor.h
		metal/TextureDecompressor.cpp
		metal/Shaders.metal
	)
	list (APPEND SOURCES ${METAL_SOURCES})
	set_source_files_properties(metal/MetalRenderer.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
endif()

if(APPLE)
	if(CMAKE_SYSTEM_NAME STREQUAL iOS)
		add_executable(MyGameEngine MACOSX_BUNDLE ${SOURCES})
		set_target_properties(MyGameEngine PROPERTIES
			MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist
		)
	else()
		add_executable(MyGameEngine MACOSX_BUNDLE ${SOURCES})
	endif()
elseif(ANDROID)
	add_library(MyGameEngine SHARED ${SOURCES})
	target_link_libraries(MyGameEngine vulkan ${log-lib})
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

if (MSVC)
	target_link_libraries(MyGameEngine ${CMAKE_HOME_DIRECTORY}/3rdparty/SDL2/lib/SDL2.lib)
	target_link_libraries(MyGameEngine ${CMAKE_HOME_DIRECTORY}/3rdparty/SDL2/lib/SDL2main.lib)
	target_link_libraries(MyGameEngine opengl32.lib)
elseif(APPLE)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework MetalKit")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal")
	if(CMAKE_SYSTEM_NAME STREQUAL iOS)
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework UIKit")
	else()
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework AppKit")
	endif()
endif()

#target_include_directories(MyGameEngine PUBLIC ${CMAKE_HOME_DIRECTORY}/3rdparty/bullet/src)
target_link_libraries(MyGameEngine BulletDynamics BulletCollision LinearMath)

target_link_libraries(MyGameEngine zlib)
target_link_libraries(MyGameEngine pugixml)
target_link_libraries(MyGameEngine glm)

if (MSVC)
	target_include_directories(MyGameEngine PUBLIC ${CMAKE_HOME_DIRECTORY}/3rdparty/SDL2/include)
endif()

target_link_libraries(MyGameEngine Detour)
target_link_libraries(MyGameEngine Recast)

target_link_libraries(MyGameEngine tiny-aes)
target_link_libraries(MyGameEngine astc)

#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyGameEngine)

add_subdirectory(SPIRV_Converter)
