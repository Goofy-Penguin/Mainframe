﻿file(GLOB_RECURSE source_files
    "src/*.cpp"
    "src/*.c"
    "include/*.hpp"
    "include/*.h"
)

set(output_target simplemenuTest)
add_executable(${output_target} ${source_files})
target_compile_features(${output_target} PRIVATE cxx_std_20)
target_link_libraries(${output_target} PRIVATE mainframe.game mainframe.math mainframe.utils mainframe.render mainframe.ui ${mf_winlibs} ${mf_ui_libs})
