file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target windowTest)
add_executable(${output_target} ${source_files})
target_include_directories(${output_target} PRIVATE "../../include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.math mainframe.utils fmt::fmt glfw GLEW::GLEW ${mf_winlibs})
