file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
	"include/*.hpp"
)

set (mf_ui_libs "")
if (use_cef)
	set (mf_ui_libs "cefdll_wrapper")
endif()

# dll
set(output_target mainframe.ui)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.math mainframe.render mainframe.game ${CONAN_LIBS} freetype-gl ${mf_winlibs} ${mf_ui_libs})

if (build_tests)
	add_subdirectory("tests")
endif()