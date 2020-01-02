#for iOS!!!
cmake_minimum_required(VERSION 3.14)

project(MyGameEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if(MSVC)
	OPTION(USE_VULKAN "Use Vulkan Renderer"	OFF)

	add_definitions(-DUNICODE)
	add_definitions(-DNOMINMAX)
	add_definitions (/D "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")
	
	message(STATUS "  Adding WIN32_LEAN_AND_MEAN to compiler defines")
	add_definitions( -DWIN32_LEAN_AND_MEAN )

	add_compile_options(
		/Zi #debug
		/Zo #Enhance Optimized Debugging
		/permissive- #Standards conformance
		/MP #Build with Multiple Processes
	)
	
	set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 /MTd /Od")
	set(CMAKE_C_FLAGS_RELEASE "/MT")
	set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=1 /Zi /MTd /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "/MT /GS-")

	# /GS- - No stack buffer overflow checks
	add_compile_options("$<$<CONFIG:Release>:/GS->")
	
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "/DEBUG /MANIFEST:NO" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/DEBUG /MANIFEST:NO /INCREMENTAL:NO /OPT:REF,ICF" CACHE STRING "" FORCE)
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)
