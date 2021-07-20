find_package(PackageHandleStandardArgs)

if (NOT DXC_SPIRV_EXECUTABLE)
    if (WIN32)
        find_program(DXC_SPIRV_EXECUTABLE dxc PATHS
            "$ENV{VULKAN_SDK}/Bin"
            NO_DEFAULT_PATH)
    else()
        find_program(DXC_SPIRV_EXECUTABLE dxc PATHS
            /usr/bin
            /usr/local/bin)
    endif()

    if (DXC_SPIRV_EXECUTABLE)
        message(STATUS "Found DXC_SPIRV executable: ${DXC_SPIRV_EXECUTABLE}.")
    endif()
endif()

find_package_handle_standard_args(DXCspirv
    REQUIRED_VARS DXC_SPIRV_EXECUTABLE
    VERSION_VAR DXC_SPIRV_VERSION
    FAIL_MESSAGE "Cannot find a SPIR-V capable DXC executable. Please provide a valid path through the DXC_SPIRV_EXECUTABLE variable."
)