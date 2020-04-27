file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target simplemenuTest)
add_executable(${output_target} ${source_files})
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_include_directories(${output_target} PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.math mainframe.utils mainframe.render mainframe.ui ${CONAN_LIBS} ${mf_winlibs} freetype-gl ${mf_ui_libs})

MFAddContent(${output_target} "fonts/*")
MFAddContent(${output_target} "textures/*")
MFAddExecutable(${output_target})
