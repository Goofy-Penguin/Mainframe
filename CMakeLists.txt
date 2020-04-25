cmake_minimum_required (VERSION 3.8)

include(FetchContent)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)

project ("mainframe" VERSION ${MAINFRAME_VERSION})
set (build_tests true CACHE BOOL "build tests")
set (build_samples true CACHE BOOL "build samples")
set (use_cef false CACHE BOOL "use cef for UI Web element")

# libraries 3th party
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_MAKEFONT OFF CACHE BOOL "" FORCE)
set(freetype-gl_USE_VAO ON CACHE BOOL "" FORCE)

 include(conanbuildinfo.cmake)
 conan_basic_setup()

set(use_cef false)
if (use_cef)
	add_compile_definitions("MAINFRAME_ENABLE_CEF")

	set(CEF_VERSION 78.3.4+ge17bba6+chromium-78.0.3904.108)

	include("cmake/cef.cmake")
	FetchContent_Declare(cefdll_wrapper GIT_REPOSITORY https://github.com/iboB/cef-cmake.git)
	FetchContent_MakeAvailable(cefdll_wrapper)

	if(CEF_CMAKE_OS_LINUX)
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(cefName cef_binary_${CEF_VERSION}_linux64)
		else()
			set(cefName cef_binary_${CEF_VERSION}_linux32)
		endif()
	elseif(CEF_CMAKE_OS_WINDOWS)
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(cefName cef_binary_${CEF_VERSION}_windows64)
		else()
			set(cefName cef_binary_${CEF_VERSION}_windows32)
		endif()
	else()
		message(FATAL_ERROR "CEF-CMake: Download platform not supported yet")
	endif()

	include_directories("${cefdll_wrapper_SOURCE_DIR}/${cefName}/include")
endif()

FetchContent_Declare(freetype-gl GIT_REPOSITORY https://github.com/rougier/freetype-gl.git)
FetchContent_MakeAvailable(freetype-gl)

if (WIN32)
	set(mf_winlibs opengl32)
else()
	set(mf_winlibs png z)
endif()

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
