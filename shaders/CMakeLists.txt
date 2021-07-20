file(GLOB shaders
	"*.hlsl"
)

if(MSVC)
	set_source_files_properties(${shaders} PROPERTIES VS_TOOL_OVERRIDE "None")
endif()

add_custom_target(HLSL_Converter 
	SOURCES ${shaders})

foreach(file ${shaders})
	get_filename_component(baseName ${file} NAME_WE)

	#HLSL->SPIRV (Vulkan)
	add_custom_command(TARGET HLSL_Converter
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T vs_6_2 -fvk-invert-y -E VSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.glsl.spv
	)
	#HLSL->SPIRV (MSL)
	add_custom_command(TARGET HLSL_Converter
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T vs_6_2 -E VSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.msl.spv
	)

	#SPIRV->MSL
	add_custom_command(TARGET HLSL_Converter
		COMMAND SPIRV_Converter GLSL ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.glsl.spv ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.glsl
	)
	#SPIRV->GLSL
	add_custom_command(TARGET HLSL_Converter
		COMMAND SPIRV_Converter MSL ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.msl.spv ${CMAKE_CURRENT_SOURCE_DIR}/${baseName}.msl
	)
endforeach()