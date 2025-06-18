################################################################################
# Get all targets form directory
#     get_all_targets(<targets> <dir>)
# Input:
#     targets - Output variable name
#     dir     - Root directory of search
################################################################################
function(get_all_targets TARGETS DIR)
    get_property(SUB_DIRS DIRECTORY ${DIR} PROPERTY SUBDIRECTORIES)

    foreach(SUB_DIR IN LISTS SUB_DIRS)
        get_all_targets(${TARGETS} ${SUB_DIR})
    endforeach()

    get_property(SUB_TARGETS DIRECTORY ${DIR} PROPERTY BUILDSYSTEM_TARGETS)
    set(${TARGETS} ${${TARGETS}} ${SUB_TARGETS} PARENT_SCOPE)
endfunction()


################################################################################
# Get source files compile options
#     get_compile_options(<options> <target>)
# Input:
#     options - Output variable name
#     target  - Source files target
################################################################################
function(get_compile_options OPTIONS TARGET)
    unset(${OPTIONS})

    get_target_property(COMPILE_OPTIONS ${TARGET} COMPILE_OPTIONS)
    if(COMPILE_OPTIONS)
        list(APPEND ${OPTIONS} ${COMPILE_OPTIONS})
    endif()

    get_target_property(COMPILE_FLAGS ${TARGET} COMPILE_FLAGS)
    if(COMPILE_FLAGS)
        list(APPEND ${OPTIONS} ${COMPILE_FLAGS})
    endif()

    get_target_property(INCLUDE_DIRECTORIES ${TARGET} INCLUDE_DIRECTORIES)
    if(INCLUDE_DIRECTORIES)
        list(TRANSFORM INCLUDE_DIRECTORIES PREPEND "-I")
        list(APPEND ${OPTIONS} ${INCLUDE_DIRECTORIES})
    endif()

    get_target_property(COMPILE_DEFINITIONS ${TARGET} COMPILE_DEFINITIONS)
    if(COMPILE_DEFINITIONS)
        list(TRANSFORM COMPILE_DEFINITIONS PREPEND "-D")
        list(APPEND ${OPTIONS} ${COMPILE_DEFINITIONS})
    endif()

    set(${OPTIONS} ${${OPTIONS}} PARENT_SCOPE)
endfunction()

################################################################################
# Compile source file to KOMDIV object
#     compile_source_file(<object> <source> <options>)
# Input:
#     object  - Output variable name
#     source  - Source file path
#     options - Compile options list name
################################################################################
function(compile_source_file OBJECT SOURCE OPTIONS)
    get_filename_component(EXTENSION ${SOURCE} LAST_EXT)
    get_filename_component(OUTPUT ${SOURCE} NAME_WLE)

    if(EXTENSION STREQUAL ".c" OR EXTENSION STREQUAL ".cpp")
        add_custom_command(
            OUTPUT ${TARGET}/${OUTPUT}.S
            COMMAND ${CMAKE_C_COMPILER} ${${OPTIONS}} ${SOURCE} -o ${TARGET}/${OUTPUT}.ll --target=mips64-unknown-gnu -nostdlib -emit-llvm -S -march=mips3 -mabi=n32 -D__R3000__ -DKOMDIV
            COMMAND ${LLVM_STATIC_COMPILER} -relocation-model=static -filetype=asm -mattr=noabicalls -meabi=gnu -o ${TARGET}/${OUTPUT}.S ${TARGET}/${OUTPUT}.ll
            DEPENDS ${SOURCE}
            VERBATIM
        )

        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
            add_custom_command(
                OUTPUT ${TARGET}/${OUTPUT}.o
                COMMAND PowerShell -Command "& {(Get-Content ${TARGET}/${OUTPUT}.S) -Replace 'mips', 'komdiv' | Set-Content ${TARGET}/${OUTPUT}.S}"
                COMMAND PowerShell -Command "& {(Get-Content ${TARGET}/${OUTPUT}.S) -Replace '@0x7000001e', '@0x1' | Set-Content ${TARGET}/${OUTPUT}.S}"
                COMMAND ${LLVM_MACHINE_CODE} -filetype=obj --arch komdiv -mcpu=komdiv3 -mattr=noabicalls -o ${TARGET}/${OUTPUT}.o ${TARGET}/${OUTPUT}.S
                DEPENDS ${TARGET}/${OUTPUT}.S
                VERBATIM
            )
        else()
            add_custom_command(
                OUTPUT ${TARGET}/${OUTPUT}.o
                COMMAND sed -i "s/mips/komdiv/g" ${TARGET}/${OUTPUT}.S
                COMMAND sed -i "s/@0x7000001e/@0x1/g" ${TARGET}/${OUTPUT}.S
                COMMAND ${LLVM_MACHINE_CODE} -filetype=obj --arch komdiv -mcpu=komdiv3 -mattr=noabicalls -o ${TARGET}/${OUTPUT}.o ${TARGET}/${OUTPUT}.S
                DEPENDS ${TARGET}/${OUTPUT}.S
                VERBATIM
            )
        endif()
    elseif(EXTENSION STREQUAL ".s" OR EXTENSION STREQUAL ".S")
        add_custom_command(
            OUTPUT ${TARGET}/${OUTPUT}.o
            COMMAND ${LLVM_MACHINE_CODE} -filetype=obj --arch komdiv -mcpu=komdiv3 -mattr=noabicalls -o ${TARGET}/${OUTPUT}.o ${SOURCE}
            DEPENDS ${SOURCE}
            VERBATIM
        )
    else()
        unset(${OBJECT} PARENT_SCOPE)
        return()
    endif()

    set(${OBJECT} ${TARGET}/${OUTPUT}.o PARENT_SCOPE)
endfunction()

################################################################################
# Compile target source files to KOMDIV objects
#     compile_target_sources(<objects> <dependencies> <target>)
# Input:
#     objects      - Generated objects output variable name
#     dependencies - Object libraries dependency output variable name
#     target       - Source files target
################################################################################
function(compile_target_sources OBJECTS DEPENDENCIES TARGET)
    get_compile_options(TARGET_OPTIONS ${TARGET})

    get_target_property(SOURCE_DIR ${TARGET} SOURCE_DIR)
    get_target_property(SOURCES ${TARGET} SOURCES)

    unset(${OBJECTS})
    unset(${DEPENDENCIES})
    foreach(SOURCE IN LISTS SOURCES)
        if(SOURCE MATCHES "\\$<TARGET_OBJECTS:.+>")
            string(LENGTH ${SOURCE} LENGTH)
            math(EXPR LENGTH "${LENGTH} - 18")
            string(SUBSTRING ${SOURCE} 17 ${LENGTH} SOURCE)
            list(APPEND ${DEPENDENCIES} ${SOURCE})
            continue()
        endif()

        file(REAL_PATH ${SOURCE} SOURCE BASE_DIRECTORY ${SOURCE_DIR})
        compile_source_file(OUTPUT ${SOURCE} TARGET_OPTIONS)
        list(APPEND ${OBJECTS} ${OUTPUT})
    endforeach()

    set(${OBJECTS} ${${OBJECTS}} PARENT_SCOPE)
    set(${DEPENDENCIES} ${${DEPENDENCIES}} PARENT_SCOPE)
endfunction()


################################################################################
# Search all KOMDIV dependency target objects
#     get_dependency_objects(<objects> <targets>)
# Input:
#     objects - Found objects output variable name
#     targets - Dependency target names
################################################################################
function(get_dependency_objects OBJECTS TARGETS)
    unset(${OBJECTS})

    foreach(TARGET IN LISTS TARGETS)
        list(APPEND ${OBJECTS} ${${TARGET}_OBJECTS})

        get_dependency_objects(${TARGET}_DEPENDENCY_OBJECTS "${${TARGET}_DEPENDENCIES}")
        list(APPEND ${OBJECTS} ${${TARGET}_DEPENDENCY_OBJECTS})
    endforeach()

    set(${OBJECTS} ${${OBJECTS}} PARENT_SCOPE)
endfunction()

################################################################################
# KOMDIV main function
################################################################################
function(main)
    cmake_path(REPLACE_FILENAME CMAKE_C_COMPILER llc OUTPUT_VARIABLE LLVM_STATIC_COMPILER)
    cmake_path(REPLACE_FILENAME CMAKE_C_COMPILER llvm-mc OUTPUT_VARIABLE LLVM_MACHINE_CODE)

    get_all_targets(ALL_TARGETS ${CMAKE_SOURCE_DIR})
    foreach(TARGET IN LISTS ALL_TARGETS)
        get_target_property(TYPE ${TARGET} TYPE)
        if(NOT TYPE STREQUAL "OBJECT_LIBRARY")
            if(TYPE STREQUAL "UTILITY")
                message(WARNING "${TYPE} target ${TARGET} cannot be converted, skipped")
            elseif(NOT TYPE STREQUAL "EXECUTABLE")
                message(SEND_ERROR "Unsupported target type ${TYPE} for ${TARGET}")
            endif()

            continue()
        endif()

        compile_target_sources(OUTPUTS LIBRARIES ${TARGET})
        set(${TARGET}_OBJECTS ${OUTPUTS})
        set(${TARGET}_DEPENDENCIES ${LIBRARIES})

        set_target_properties(${TARGET} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endforeach()

    if(NOT NO_EMUL)
        cmake_path(REPLACE_FILENAME CMAKE_TOOLCHAIN_FILE "emul.lib.S" OUTPUT_VARIABLE EMUL_LIB)
        set(TARGET EMUL)
        compile_source_file(EMUL ${EMUL_LIB} "")
    endif()

    foreach(TARGET IN LISTS ALL_TARGETS)
        get_target_property(TYPE ${TARGET} TYPE)
        if(NOT TYPE STREQUAL "EXECUTABLE")
            continue()
        endif()

        compile_target_sources(OUTPUTS LIBRARIES ${TARGET})
        get_dependency_objects(LIBRARIES "${LIBRARIES}")

        add_executable(${TARGET}_KOMDIV
            ${OUTPUTS}
            ${LIBRARIES}
            ${EMUL}
        )

        target_link_options(${TARGET}_KOMDIV PRIVATE
            --target=mips64-unknown-gnu
            -nostdlib
            -fuse-ld=lld
            -Wl,-e,main
            -Wl,-Ttext=0x80010000
            -Wl,-melf32btsmip
        )

        set_target_properties(${TARGET}_KOMDIV PROPERTIES
            LINKER_LANGUAGE C
            SUFFIX ""
        )

        set_target_properties(${TARGET} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endforeach()
endfunction()

################################################################################
# System call at the end of project configuring
################################################################################
function(eof_call)
    if(NOT IDS OR NO_KOMDIV)
        return()
    endif()

    main()
    cmake_language(DEFER CANCEL_CALL ${EOF_ID})
endfunction()

cmake_language(DEFER GET_CALL_IDS IDS)
cmake_language(DEFER ID_VAR EOF_ID CALL eof_call)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_SYSTEM_NAME Generic)
