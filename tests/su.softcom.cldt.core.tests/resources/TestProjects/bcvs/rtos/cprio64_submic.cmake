cmake_minimum_required(VERSION 3.16.0)

include(baget-tools)

################################################################################
# Configuration
################################################################################
set(BSP cprio64_submic)
set(OSBASE ${CMAKE_CURRENT_LIST_DIR}/2.75.002/komdiv64)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${OSBASE}/bsp/${BSP}/lib/include;
    ${OSBASE}/oc-include;
    ${OSBASE}/include;
    ${OSBASE}/include/opt;
    ${OSBASE}/bsp;
    ${OSBASE}/bsp/${BSP};
    ${OSBASE}/bsp/${BSP}/include;
    ${OSBASE}/locale;
)

################################################################################
# Relocatable output
################################################################################
set(RELOCATABLE tmp)
set(ARCHIVES
    ${OSBASE}/lib/bsp${BSP}.a
    ${OSBASE}/lib/oc2000komdiv64.a
    ${OSBASE}/lib/libmK64.a
)

target_sources(${PROJECT_NAME} PRIVATE
    ${OSBASE}/bsp/oc2000.c
    ${OSBASE}/bsp/${BSP}/cpuLib.S
    ${OSBASE}/bsp/${BSP}/atrace.S
)

add_custom_command(
    OUTPUT ${RELOCATABLE}
    COMMAND ${CMAKE_LINKER} -r $<TARGET_OBJECTS:${PROJECT_NAME}> -o ${RELOCATABLE} -'\(' ${ARCHIVES} -'\)'
    DEPENDS $<TARGET_OBJECTS:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)

set_source_files_properties(${RELOCATABLE} PROPERTIES GENERATED "TRUE")
add_custom_target(${PROJECT_NAME}_RL DEPENDS ${RELOCATABLE})
add_dependencies(${PROJECT_NAME}_RL ${PROJECT_NAME})

################################################################################
# Symbol table entries
################################################################################
set(SYMBOL_TABLE sym_tbl.s)

add_custom_command(
    OUTPUT ${SYMBOL_TABLE}
    COMMAND ${CMAKE_OBJDUMP} --syms ${RELOCATABLE} > out
    COMMAND ${CMAKE_COMMAND} -E copy ${OSBASE}/lib/oooR4000.def out1
    COMMAND ${OSBASE}/bin/elfhR4000 ${RELOCATABLE} >> out1
    COMMAND gawk -f ${OSBASE}/lib/def.awk out1 out | sort | gawk -f ${OSBASE}/lib/sort.awk -v cpu=R4000 > ${SYMBOL_TABLE}
    DEPENDS ${RELOCATABLE}
    COMMAND_EXPAND_LISTS
)

set_source_files_properties(${SYMBOL_TABLE} PROPERTIES GENERATED "TRUE")
add_custom_target(${PROJECT_NAME}_ST DEPENDS ${SYMBOL_TABLE})
add_dependencies(${PROJECT_NAME}_ST ${PROJECT_NAME}_RL)

################################################################################
# Initialization objects
################################################################################
add_library(${PROJECT_NAME}_INI OBJECT
    ${OSBASE}/bsp/${BSP}/cpuinit.s
    ${SYMBOL_TABLE}
)

get_target_property(OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
target_compile_options(${PROJECT_NAME}_INI PRIVATE
    ${OPTIONS}
    -x assembler-with-cpp
)

get_target_property(DEFINITIONS ${PROJECT_NAME} COMPILE_DEFINITIONS)
target_compile_definitions(${PROJECT_NAME}_INI PRIVATE
    ${DEFINITIONS}
    _ASM
)

add_dependencies(${PROJECT_NAME}_INI ${PROJECT_NAME}_ST)


################################################################################
# Operating system
################################################################################
add_executable(${PROJECT_NAME}_OS
    $<TARGET_OBJECTS:${PROJECT_NAME}_INI>
    ${RELOCATABLE}
    ${OSBASE}/lib/setdef0R4000.o
    ${OSBASE}/lib/setdef1R4000.o
)

set_target_properties(${PROJECT_NAME}_OS PROPERTIES
    OUTPUT_NAME oc2000
    LINKER_LANGUAGE C
)

target_compile_options(${PROJECT_NAME}_OS PRIVATE
    -nostartfiles -N
    -fno-common -fshort-wchar
    -e cpuInit
    -Xlinker -Ttext
    -Xlinker 0x80010000 
    -lgcc
)

add_dependencies(${PROJECT_NAME}_OS ${PROJECT_NAME}_INI)
