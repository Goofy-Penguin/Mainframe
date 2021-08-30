file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
	"include/*.h"
)

# dll
set(output_target mainframe.render)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PUBLIC mainframe.math ${mf_winlibs} GLEW::GLEW fmt::fmt lodepng::lodepng Freetype::Freetype utf8cpp)

if (build_tests)
	add_subdirectory("tests")
endif()
