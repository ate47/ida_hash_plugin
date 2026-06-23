# MockExe
file(GLOB_RECURSE MSL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/mockexe/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/mockexe/*.h"
    "${CMAKE_SOURCE_DIR}/src/mockexe/*.hpp"
)
add_executable(MockExe ${MSL_SOURCES})
set_target_properties(MockExe PROPERTIES
    OUTPUT_NAME "hash-mockexe"
    FOLDER "Plugin"
)
source_group(
    TREE "${CMAKE_SOURCE_DIR}/src/mockexe"
    PREFIX mockexe
    FILES ${MSL_SOURCES}
)

append_common_defs(MockExe)

target_include_directories(MockExe PRIVATE 
    "${CMAKE_SOURCE_DIR}/src/mockexe"
)
