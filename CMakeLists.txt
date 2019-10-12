cmake_minimum_required (VERSION 3.8)

project ("mainframe" VERSION 1.0.0)
set (build_tests true CACHE BOOL "build tests")
set (build_samples true CACHE BOOL "build samples")
#set (glew_dir "NOT_FOUND" CACHE STRING "glew directory")

set(MY_LIBRARY_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(MY_LIBRARY_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(MY_LIBRARY_VERSION ${MY_LIBRARY_VERSION_MAJOR}.${MY_LIBRARY_VERSION_MINOR} CACHE STRING "version" FORCE)


# libraries
find_package(fmt REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(glew REQUIRED)
#add_subdirectory(libs/glfw)

# dlls
add_subdirectory ("utils")
#add_subdirectory ("game") # glew/glfw issues
add_subdirectory ("networking")

# samples
if (build_samples)
	#add_subdirectory ("samples")
endif()
