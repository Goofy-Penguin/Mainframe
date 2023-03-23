file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

# dll
set(output_target mainframe.ui)
add_library(${output_target} STATIC ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PUBLIC cxx_std_20)
target_link_libraries(${output_target} PUBLIC mainframe.game ${mf_winlibs} ${mf_ui_libs} mainframe.utils mainframe.render mainframe.math)

if (build_tests)
	add_subdirectory("tests")
endif()
