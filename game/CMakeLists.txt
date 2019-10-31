file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

# dll
set(output_target mainframe.game)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include" "../utils/include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.math glfw GLEW::GLEW opengl32)

if (build_tests)
	add_subdirectory("tests")
endif()