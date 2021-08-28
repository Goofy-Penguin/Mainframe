cmake_minimum_required (VERSION 3.8)

include(FetchContent)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)
set(MAINFRAME_FREETYPE_GL_VERSION "637599f58401be69bafc5332d7af6d272a5aed6e" CACHE STRING "the tag used for freetype-gl")

include("cmake/HunterGate.cmake")
HunterGate(
	URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
	SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
)
project ("mainframe" VERSION ${MAINFRAME_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_FMT_EXTERNAL -DNOMINMAX")

hunter_add_package(glfw)
hunter_add_package(GLEW)
hunter_add_package(fmt)
hunter_add_package(freetype)
hunter_add_package(PNG)
hunter_add_package(ZLIB)
hunter_add_package(BZip2)
hunter_add_package(nlohmann_json)
hunter_add_package(glm)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(fmt REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glm REQUIRED)

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
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_USE_VAO ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(freetype-gl)
if(NOT freetype-gl_POPULATED)
	FetchContent_Declare(freetype-gl GIT_REPOSITORY https://github.com/Bromvlieg/freetype-gl.git)
	FetchContent_MakeAvailable(freetype-gl)
endif()

if (WIN32)
	set(mf_winlibs opengl32)
else()
	set(mf_winlibs png z)
endif()

# helpers
function(MFAddContent projname filesfilter)
	message("")
endfunction()

function(MFAddExecutable projname)
	message("detected exe:" ${CMAKE_BINARY_DIR}/bin/${output_target})
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
