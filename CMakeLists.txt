cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)

project ("mainframe" VERSION ${MAINFRAME_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_FMT_EXTERNAL -DNOMINMAX")
set(CMAKE_CXX_STANDARD 23)
set (build_tests true CACHE BOOL "build tests")
set (build_samples true CACHE BOOL "build samples")

# libraries 3th party
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (WIN32)
	set(mf_winlibs opengl32)
else()
	set(mf_winlibs z)
endif()

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(fmt REQUIRED)
find_package(Freetype REQUIRED MODULE)
find_package(nlohmann_json REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(glm REQUIRED)

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
