file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

# dll
set(output_target mainframe.render)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include" ${FREETYPE_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/_deps/freetype-gl-src)
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PUBLIC mainframe.math ${mf_winlibs} glew::glew freetype-gl ${FREETYPE_LIBRARIES} PNG::PNG ZLIB::ZLIB BZip2::BZip2)

if (build_tests)
	add_subdirectory("tests")
endif()
