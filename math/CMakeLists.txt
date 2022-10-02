file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
	"include/*.h"
)

# dll
set(output_target mainframe.math)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_link_libraries(${output_target} PUBLIC glm::glm)
target_compile_features(${output_target} PUBLIC cxx_std_20)
target_compile_definitions(${output_target} PUBLIC
	GLM_FORCE_LEFT_HANDED
)

if (build_tests)
	add_subdirectory("tests")
endif()
