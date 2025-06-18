cmake_minimum_required(VERSION 3.16.0)

################################################################################
# Remove unsupported
################################################################################

get_target_property(COMPILE_OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)

list(REMOVE_ITEM COMPILE_OPTIONS
    -k64rio
    -mno-fused-madd
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")

################################################################################
# Configuration
################################################################################
set(BSP cprio64_submic)
set(OSBASE ${CMAKE_CURRENT_LIST_DIR}/../rtos/2.75.002/komdiv64)

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
# Operating system
################################################################################
add_executable(${PROJECT_NAME}_OS
    $<TARGET_OBJECTS:${PROJECT_NAME}>
	${CMAKE_CURRENT_LIST_DIR}/vm6.c
)

set_target_properties(${PROJECT_NAME}_OS PROPERTIES
    OUTPUT_NAME oc2000
    LINKER_LANGUAGE C
)

add_dependencies(${PROJECT_NAME}_OS ${PROJECT_NAME})
