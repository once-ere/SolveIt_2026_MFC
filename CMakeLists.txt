cmake_minimum_required(VERSION 3.28)

project(SolveIt_2026_MFC LANGUAGES NONE)

if(NOT WIN32)
    message(FATAL_ERROR "SolveIt_2026_MFC currently supports Windows only.")
endif()

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported configurations" FORCE)
endif()

find_program(SOLVEIT_MSBUILD_EXECUTABLE
    NAMES MSBuild.exe MSBuild
    HINTS
        "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin"
        "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Professional/MSBuild/Current/Bin"
        "$ENV{ProgramFiles}/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin"
)

if(NOT SOLVEIT_MSBUILD_EXECUTABLE)
    message(FATAL_ERROR "MSBuild.exe was not found. Install Visual Studio 2022 Build Tools or run from a Developer Command Prompt.")
endif()

set(SOLVEIT_PROJECT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/SolveIt.vcxproj")
set(SOLVEIT_OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/x64/$<CONFIG>/SolveIt.exe")

add_custom_target(SolveIt ALL
    COMMAND "${SOLVEIT_MSBUILD_EXECUTABLE}" "${SOLVEIT_PROJECT_FILE}" /p:Configuration=$<CONFIG> /p:Platform=x64 /m /nologo
    BYPRODUCTS "${SOLVEIT_OUTPUT_FILE}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)

add_custom_target(SolveItExe DEPENDS SolveIt)
