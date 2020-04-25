file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target stencilTest)
add_executable(${output_target} ${source_files})
target_include_directories(${output_target} PRIVATE "../../include" ${FREETYPE_INCLUDE_DIRS})
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.math mainframe.utils mainframe.render fmt::fmt glfw GLEW::GLEW freetype-gl ${mf_winlibs} ${FREETYPE_LIBRARIES})
