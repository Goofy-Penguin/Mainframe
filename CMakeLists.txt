cmake_minimum_required (VERSION 3.8)

include(FetchContent)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)

include("cmake/HunterGate.cmake")
HunterGate(
	URL "https://github.com/cpp-pm/hunter/archive/v0.23.254.tar.gz"
	SHA1 "017e688b9848c1f695303e8cfe9f35d40e154112"
)

project ("mainframe" VERSION ${MAINFRAME_VERSION})

hunter_add_package(glfw)
hunter_add_package(glew)
hunter_add_package(fmt)
hunter_add_package(freetype)
hunter_add_package(PNG)
hunter_add_package(ZLIB)
hunter_add_package(BZip2)
hunter_add_package(nlohmann_json)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(fmt REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(nlohmann_json REQUIRED)

set (build_tests true CACHE BOOL "build tests")
set (build_samples true CACHE BOOL "build samples")

# libraries 3th party
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_MAKEFONT OFF CACHE BOOL "" FORCE)
set(freetype-gl_USE_VAO ON CACHE BOOL "" FORCE)

FetchContent_Declare(freetype-gl GIT_REPOSITORY https://github.com/rougier/freetype-gl.git)
FetchContent_MakeAvailable(freetype-gl)

if (WIN32)
	set(mf_winlibs opengl32)
else()
	set(mf_winlibs png z)
endif()

# helpers
function(MFAddContent projname filesfilter)
	message("TODO: Fix me brom (MFAddContent)")

	#file(GLOB_RECURSE fileslst ${filesfilter})
	#foreach(contentfile IN LISTS fileslst)
	#	STRING(REGEX REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" contentfile_stripped ${contentfile})
	#	message("detected content:" ${contentfile_stripped})
#
	#	add_custom_command(
	#		TARGET ${projname}
	#		POST_BUILD
	#		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${contentfile} ${CMAKE_CURRENT_BINARY_DIR}/bin${contentfile_stripped})
	#endforeach()
endfunction()

function(MFAddExecutable projname)
	message("detected exe:" ${CMAKE_BINARY_DIR}/bin/${output_target})
	
	if (WIN32)
		#add_custom_command(
		#	TARGET ${output_target}
		#	POST_BUILD
		#	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/${output_target}.exe ${CMAKE_SOURCE_DIR}/bin/${output_target}.exe)
	else()
		#add_custom_command(
		#	TARGET ${output_target}
		#	POST_BUILD
		#	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${output_target} ${CMAKE_SOURCE_DIR}/bin/${output_target})
	endif()
endfunction()

# our libs
add_subdirectory ("utils")
add_subdirectory ("math")
add_subdirectory ("networking")
add_subdirectory ("database")
add_subdirectory ("game")
add_subdirectory ("render")
add_subdirectory ("ui")

# samples
if (build_samples)
	#add_subdirectory ("samples")
endif()
