# IDAHashPlugin
file(GLOB_RECURSE IHP_SOURCES
    "${CMAKE_SOURCE_DIR}/src/plugin/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/plugin/*.h"
    "${CMAKE_SOURCE_DIR}/src/plugin/*.hpp"
)

macro(make_ihp_target target sdk hexrays output_file)
    add_library(${target} SHARED)
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${output_file}"
        FOLDER "Plugin"
    )
    target_sources(${target} PRIVATE ${IHP_SOURCES})
    source_group(
        TREE "${CMAKE_SOURCE_DIR}/src/plugin"
        PREFIX plugin
        FILES ${IHP_SOURCES}
    )

    append_common_defs(${target})

    target_include_directories(${target} PRIVATE 
        "${CMAKE_SOURCE_DIR}/src/plugin"
        "${CMAKE_SOURCE_DIR}/deps/lz4/lib/"
        "${sdk}/include"
        "${hexrays}/include"
    )

    target_link_libraries(${target} PRIVATE
        lz4
        "${sdk}/lib/x64_win_vc_64/ida.lib"
    )

    add_dependencies(${target}
        lz4
    )

    target_compile_definitions(${target} PRIVATE 
        __EA64__
        __NT__
        USE_STANDARD_FILE_FUNCTIONS
    )
endmacro()

make_ihp_target(IDAHashPlugin ${IDASDK_DIR} ${HEXRAYS_SDK_DIR} "ida_hash_plugin64")
make_ihp_target(IDAHashPlugin9 ${IDASDK9_DIR} ${IDASDK9_DIR} "ida_hash_plugin")