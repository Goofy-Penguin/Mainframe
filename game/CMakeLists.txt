file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.h"
)

# dll
set(output_target mainframe.game)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE glew::glew ${mf_winlibs} PUBLIC mainframe.networking mainframe.math mainframe.utils glfw)

if (build_tests)
	add_subdirectory("tests")
endif()
