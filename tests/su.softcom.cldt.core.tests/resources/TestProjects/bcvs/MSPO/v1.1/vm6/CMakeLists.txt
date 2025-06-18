cmake_minimum_required(VERSION 3.16.0)

project(VM6)


file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.c" "src/*.S")
add_library(${PROJECT_NAME} OBJECT ${SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../1890VM6/lib1890VM6.o
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../1890VM6/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)


string(TIMESTAMP IMAGE_DATE "%a %b %d %H:%M:%S UTC %Y" UTC)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CPU=R4000
    ROM_TEXT_ADRS=0x98300000
    RAM_LOW_ADRS=0x80010000
    STARTPOINT=0x80010000
    RAM_COMP_ADRS
    COMPRESS
    K64SMP
    K64RIO
    CPRIO64
    SUBMICRON
    FOR_SUBMICRON
    NAME_MAX=80
    CLOCK_MAX=4
    SIGQUEUE_MAX=32
    IMAGE_DATE="${IMAGE_DATE}"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -g
    -O0
    -G0
    -pipe
    -k64rio
    -nostdlib
    -fshort-wchar
    -fno-common
    -fno-builtin-sqrt
    -fno-strict-aliasing
    -fno-merge-constants
    -mno-fused-madd
    -Wall
    -Wno-pointer-sign
    -Wno-trigraphs
)


include(cprio64_submic)
