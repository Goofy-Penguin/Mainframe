cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

include(FetchContent)

set(MAINFRAME_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MAINFRAME_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MAINFRAME_VERSION ${MAINFRAME_VERSION_MAJOR}.${MAINFRAME_VERSION_MINOR} CACHE STRING "version" FORCE)
set(MAINFRAME_FREETYPE_GL_VERSION "637599f58401be69bafc5332d7af6d272a5aed6e" CACHE STRING "the tag used for freetype-gl")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_FMT_EXTERNAL -DNOMINMAX")
set(build_tests true CACHE BOOL "build tests")
set(build_samples true CACHE BOOL "build samples")

# libraries 3th party
set(freetype-gl_BUILD_APIDOC OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_MAKEFONT OFF CACHE BOOL "" FORCE)
set(freetype-gl_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(freetype-gl_USE_VAO ON CACHE BOOL "" FORCE)

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

project ("mainframe" VERSION ${MAINFRAME_VERSION})
include("cmake/CPM.cmake")

CPMAddPackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.4
        OPTIONS
          "GLFW_BUILD_TESTS OFF"
          "GLFW_BUILD_EXAMPLES OFF"
          "GLFW_BULID_DOCS OFF"
)

CPMAddPackage("gh:nigels-com/glew#glew-2.2.0")
CPMAddPackage(
  NAME fmt
  GIT_TAG 6.1.2
  GITHUB_REPOSITORY fmtlib/fmt
)

CPMAddPackage("gh:opencor/bzip2#87855c58afc9e8e7d579c97dd867ec20709b3eb0")
CPMAddPackage("gh:madler/zlib@1.2.11")
CPMAddPackage("gh:glennrp/libpng@1.6.36")

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.7.3
  # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.7.3/include.zip
  URL_HASH SHA256=87b5884741427220d3a33df1363ae0e8b898099fbc59f1c451113f6732891014
)

if (nlohmann_json_ADDED)
  add_library(nlohmann_json INTERFACE IMPORTED)
  target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR}/include)
endif()

CPMAddPackage("gh:g-truc/glm#0.9.9.8")

CPMAddPackage(
  NAME freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-10-0
  VERSION 2.10.0
)

if (freetype_ADDED)
  add_library(Freetype::Freetype ALIAS freetype)
endif()

CPMAddPackage("gh:Bromvlieg/freetype-gl#master")

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
