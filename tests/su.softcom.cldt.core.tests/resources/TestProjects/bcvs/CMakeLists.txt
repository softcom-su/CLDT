cmake_minimum_required(VERSION 3.16.0)

project(BCVS)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CMAKE_CURRENT_SOURCE_DIR}/rtos
    ${CMAKE_CURRENT_SOURCE_DIR}/plug
)


add_subdirectory(MSPO/v1.1/vm6)

