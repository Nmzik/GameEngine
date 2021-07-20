file(GLOB shaders
	"*.hlsl"
)

if(MSVC)
	set_source_files_properties(${shaders} PROPERTIES VS_TOOL_OVERRIDE "None")
endif()

add_custom_target(HLSL_Converter 
	SOURCES ${shaders})

add_custom_command(TARGET HLSL_Converter PRE_BUILD 
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/Metal/)

foreach(file ${shaders})
	get_filename_component(baseName ${file} NAME_WE)

	#HLSL->SPIRV (Vulkan)
	add_custom_command(TARGET HLSL_Converter
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T vs_6_2 -fvk-invert-y -E VSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.vs.spv
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T ps_6_2 -E PSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.ps.spv
	)
	#SPIRV->GLSL
	add_custom_command(TARGET HLSL_Converter
		COMMAND SPIRV_Converter GLSL ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.vs.spv ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.vs.glsl
		COMMAND SPIRV_Converter GLSL ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.ps.spv ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan/${baseName}.ps.glsl
	)

	#HLSL->SPIRV (MSL)
	add_custom_command(TARGET HLSL_Converter
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T vs_6_2 -E VSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.vs.spv
		COMMAND ${DXC_SPIRV_EXECUTABLE} -spirv -T ps_6_2 -E PSMain ${file} -Fo ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.ps.spv
	)
	#SPIRV->MSL
	add_custom_command(TARGET HLSL_Converter
		COMMAND SPIRV_Converter MSL ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.vs.spv ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.vs.msl
		COMMAND SPIRV_Converter MSL ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.ps.spv ${CMAKE_CURRENT_SOURCE_DIR}/Metal/${baseName}.ps.msl
	)
endforeach()