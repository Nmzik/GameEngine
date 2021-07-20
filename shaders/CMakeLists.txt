file(GLOB shaders
	"*.hlsl"
)

if(MSVC)
	set_source_files_properties(${shaders} PROPERTIES VS_TOOL_OVERRIDE "None")
endif()

add_custom_target(HLSL_TO_SPIRV 
	SOURCES ${shaders})

foreach(file ${shaders})
	get_filename_component(baseName ${file} NAME_WE)

	add_custom_command(TARGET HLSL_TO_SPIRV
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T vs_6_2 -fvk-invert-y -O3 -Zpc -E VSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.spv
	)
endforeach()