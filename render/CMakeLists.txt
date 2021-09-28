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
target_compile_features(${output_target} PRIVATE cxx_std_20)

if (WIN32)
	target_link_libraries(${output_target} PUBLIC mainframe.math ${mf_winlibs} GLEW freetype PNG::PNG lodepng::lodepng fmt::fmt ZLIB::ZLIB BZip2::BZip2 Freetype::Freetype utf8cpp)
else()
	target_link_libraries(${output_target} PUBLIC mainframe.math ${mf_winlibs} GLEW PNG::PNG lodepng::lodepng ZLIB::ZLIB BZip2::BZip2 Freetype::Freetype utf8cpp)
endif()

if (build_tests)
	add_subdirectory("tests")
endif()
