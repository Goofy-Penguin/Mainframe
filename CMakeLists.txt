cmake_minimum_required (VERSION 3.8)

include(FetchContent)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)

project ("mainframe" VERSION ${MAINFRAME_VERSION})
set (build_tests true CACHE BOOL "build tests")
set (build_samples true CACHE BOOL "build samples")

# libraries 3th party
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_MAKEFONT OFF CACHE BOOL "" FORCE)
set(freetype-gl_USE_VAO ON CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(freetype REQUIRED)

FetchContent_Declare(fmtlib GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(fmtlib)

FetchContent_Declare(glfwlib GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_MakeAvailable(glfwlib)

FetchContent_Declare(glewlib GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git)
FetchContent_MakeAvailable(glewlib)

FetchContent_Declare(freetypegllib GIT_REPOSITORY https://github.com/rougier/freetype-gl.git)
FetchContent_MakeAvailable(freetypegllib)

# our libs
add_subdirectory ("utils")
add_subdirectory ("math")
add_subdirectory ("networking")
add_subdirectory ("database")
add_subdirectory ("game")
add_subdirectory ("render")

# samples
if (build_samples)
	#add_subdirectory ("samples")
endif()
