file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

# dll
set(output_target mainframe.database)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_20)
target_link_libraries(${output_target} PUBLIC fmt::fmt mainframe.networking)

if (build_tests)
	add_subdirectory("tests")
endif()
