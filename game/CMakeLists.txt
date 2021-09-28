file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.h"
)

# dll
set(output_target mainframe.game)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_20)


if (WIN32)
	target_link_libraries(${output_target} PUBLIC glew::glew ${mf_winlibs} PRIVATE fmt::fmt PUBLIC mainframe.networking mainframe.math mainframe.utils glfw)
else()
	target_link_libraries(${output_target} PUBLIC GLEW GL ${mf_winlibs} PRIVATE fmt::fmt PUBLIC mainframe.networking mainframe.math mainframe.utils glfw)
endif()

if (build_tests)
	add_subdirectory("tests")
endif()
