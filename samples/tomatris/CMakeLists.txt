project("tomatris")

file(GLOB source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target tomatris)
add_executable(${output_target} ${source_files})
target_include_directories(${output_target} PRIVATE "include")
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.utils)
