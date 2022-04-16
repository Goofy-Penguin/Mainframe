file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

# dll
set(output_target mainframe.networking)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PUBLIC cxx_std_20)

if (build_tests)
	add_subdirectory("tests")
endif()
