file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.c"
)

set(output_target mainframe.utils)
add_library(${output_target} ${source_files})
target_include_directories(${output_target} PUBLIC "include")
target_compile_features(${output_target} PRIVATE cxx_std_20)
set_target_properties(${output_target} PROPERTIES LINKER_LANGUAGE CXX)

if (WIN32)
	target_link_libraries(${output_target} PRIVATE fmt)
else()
	target_link_libraries(${output_target} PRIVATE fmt pthread)
endif()
