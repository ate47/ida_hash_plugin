# IDAHashPlugin
file(GLOB_RECURSE IHP_SOURCES
    "${CMAKE_SOURCE_DIR}/src/plugin/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/plugin/*.h"
    "${CMAKE_SOURCE_DIR}/src/plugin/*.hpp"
)
add_library(IDAHashPlugin SHARED)
set_target_properties(IDAHashPlugin PROPERTIES
    OUTPUT_NAME "ida_hash_plugin64"
    FOLDER "Plugin"
)
target_sources(IDAHashPlugin PRIVATE ${IHP_SOURCES})
source_group(
    TREE "${CMAKE_SOURCE_DIR}/src/plugin"
    PREFIX plugin
    FILES ${IHP_SOURCES}
)

append_common_defs(IDAHashPlugin)

target_include_directories(IDAHashPlugin PRIVATE 
    "${CMAKE_SOURCE_DIR}/src/plugin"
    "${CMAKE_SOURCE_DIR}/deps/lz4/lib/"
    "${IDASDK_DIR}/include"
    "${HEXRAYS_SDK_DIR}/include"
)

target_link_libraries(IDAHashPlugin PRIVATE
    lz4
    "${IDASDK_DIR}/lib/x64_win_vc_64/ida.lib"
)

add_dependencies(IDAHashPlugin
    lz4
)

target_compile_definitions(IDAHashPlugin PRIVATE 
    __EA64__
    __NT__
)
