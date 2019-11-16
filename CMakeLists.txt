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

find_package(OpenGL REQUIRED)
find_package(freetype REQUIRED)

if (use_cef)
	add_compile_definitions("MAINFRAME_ENABLE_CEF")

	set(CEF_VERSION 75.1.4+g4210896+chromium-75.0.3770.100)

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

FetchContent_Declare(fmtlib GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(fmtlib)

FetchContent_Declare(glfwlib GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfwlib)

FetchContent_Declare(glewlib GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git)
FetchContent_MakeAvailable(glewlib)

FetchContent_Declare(freetype-gl GIT_REPOSITORY https://github.com/rougier/freetype-gl.git)
FetchContent_MakeAvailable(freetype-gl)

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
