file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
)

set(output_target testPacket)
add_executable(${output_target} ${source_files})
target_compile_features(${output_target} PRIVATE cxx_std_17)
target_link_libraries(${output_target} PRIVATE fmt::fmt mainframe.networking)

MFAddExecutable(${output_target})
