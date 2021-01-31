file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target stencilTest)
add_executable(${output_target} ${source_files})
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.math mainframe.render ${mf_winlibs})

MFAddContent(${output_target} "fonts/*")
MFAddContent(${output_target} "textures/*")
MFAddExecutable(${output_target})
