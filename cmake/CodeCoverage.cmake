# CodeCoverage.cmake - Code coverage support for CMake
#
# Usage:
#   cmake -B build -DUNITS_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build
#   cmake --build build --target coverage
#
# Requires: lcov, genhtml (or llvm-cov for Clang)

option(UNITS_ENABLE_COVERAGE "Enable code coverage" OFF)

set(COVERAGE_TOOLS_FOUND FALSE)

if(UNITS_ENABLE_COVERAGE)
    # Coverage only works with Debug builds
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Code coverage requires Debug build type. Set -DCMAKE_BUILD_TYPE=Debug")
    endif()

    # Detect compiler and set appropriate flags
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(COVERAGE_COMPILER_FLAGS "--coverage" "-fprofile-arcs" "-ftest-coverage")
        set(COVERAGE_LINKER_FLAGS "--coverage")
        set(COVERAGE_TOOL "gcov")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(COVERAGE_COMPILER_FLAGS "-fprofile-instr-generate" "-fcoverage-mapping")
        set(COVERAGE_LINKER_FLAGS "-fprofile-instr-generate")
        set(COVERAGE_TOOL "llvm-cov")
    else()
        message(WARNING "Code coverage is not supported for ${CMAKE_CXX_COMPILER_ID}")
        set(UNITS_ENABLE_COVERAGE OFF)
    endif()

    if(UNITS_ENABLE_COVERAGE)
        # Apply coverage flags globally
        add_compile_options(${COVERAGE_COMPILER_FLAGS})
        add_link_options(${COVERAGE_LINKER_FLAGS})

        message(STATUS "Code coverage enabled with ${COVERAGE_TOOL}")

        # Find required tools
        if(COVERAGE_TOOL STREQUAL "gcov")
            find_program(LCOV_PATH lcov)
            find_program(GENHTML_PATH genhtml)
            find_program(GCOV_PATH gcov)

            if(NOT LCOV_PATH OR NOT GENHTML_PATH)
                message(WARNING "lcov/genhtml not found! Coverage target will not be available.")
                message(WARNING "Install with: apt install lcov / dnf install lcov")
            else()
                set(COVERAGE_TOOLS_FOUND TRUE)
            endif()
        else()
            find_program(LLVM_COV_PATH llvm-cov)
            find_program(LLVM_PROFDATA_PATH llvm-profdata)

            if(NOT LLVM_COV_PATH OR NOT LLVM_PROFDATA_PATH)
                message(WARNING "llvm-cov/llvm-profdata not found! Coverage target will not be available.")
                message(WARNING "Install LLVM tools.")
            else()
                set(COVERAGE_TOOLS_FOUND TRUE)
            endif()
        endif()
    endif()
endif()

# Function to add coverage target
# Usage: add_coverage_target(target_name test_executables...)
function(add_coverage_target TARGET_NAME)
    if(NOT UNITS_ENABLE_COVERAGE OR NOT COVERAGE_TOOLS_FOUND)
        return()
    endif()

    set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")
    set(COVERAGE_INFO "${COVERAGE_DIR}/coverage.info")
    set(COVERAGE_LCOV "${COVERAGE_DIR}/coverage.lcov")
    set(COVERAGE_HTML "${COVERAGE_DIR}/html")

    if(COVERAGE_TOOL STREQUAL "gcov")
        # GCC/gcov + lcov workflow
        add_custom_target(${TARGET_NAME}
            DEPENDS ${ARGN}

            # Clean previous coverage data
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${COVERAGE_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}

            # Reset counters
            COMMAND ${LCOV_PATH} --zerocounters --directory ${CMAKE_BINARY_DIR}

            # Capture baseline (before tests)
            COMMAND ${LCOV_PATH} --capture --initial
                --directory ${CMAKE_BINARY_DIR}
                --output-file ${COVERAGE_DIR}/baseline.info
                --ignore-errors mismatch,source
                --rc geninfo_unexecuted_blocks=1

            # Run all test executables
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j4

            # Capture coverage after tests
            COMMAND ${LCOV_PATH} --capture
                --directory ${CMAKE_BINARY_DIR}
                --output-file ${COVERAGE_DIR}/test.info
                --ignore-errors mismatch,source
                --rc geninfo_unexecuted_blocks=1

            # Combine baseline and test coverage
            COMMAND ${LCOV_PATH}
                --add-tracefile ${COVERAGE_DIR}/baseline.info
                --add-tracefile ${COVERAGE_DIR}/test.info
                --output-file ${COVERAGE_INFO}
                --ignore-errors mismatch,source

            # Remove system headers and external code
            COMMAND ${LCOV_PATH} --remove ${COVERAGE_INFO}
                "/usr/*"
                "*/catch2/*"
                "*/Catch2/*"
                "*/_deps/*"
                "*/tests/*"
                --output-file ${COVERAGE_INFO}
                --ignore-errors mismatch,source,unused

            # Generate HTML report
            COMMAND ${GENHTML_PATH} ${COVERAGE_INFO}
                --output-directory ${COVERAGE_HTML}
                --title "Units Library Coverage"
                --legend
                --show-details
                --demangle-cpp

            # Copy to .lcov for Codecov compatibility
            COMMAND ${CMAKE_COMMAND} -E copy ${COVERAGE_INFO} ${COVERAGE_LCOV}

            COMMAND ${CMAKE_COMMAND} -E echo "Coverage report: ${COVERAGE_HTML}/index.html"
            COMMAND ${CMAKE_COMMAND} -E echo "LCOV export: ${COVERAGE_LCOV}"

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report..."
            VERBATIM
        )
    else()
        # Clang/llvm-cov workflow
        set(PROFDATA_FILE "${COVERAGE_DIR}/coverage.profdata")

        # Build list of test executables for llvm-cov (use generator expressions)
        set(TEST_OBJECTS "")
        foreach(TEST_EXE ${ARGN})
            list(APPEND TEST_OBJECTS "-object=$<TARGET_FILE:${TEST_EXE}>")
        endforeach()
        string(JOIN " " TEST_OBJECTS_STR ${TEST_OBJECTS})

        # Create a shell script to handle the coverage workflow (uses file(GENERATE) for generator expressions)
        set(COVERAGE_SCRIPT "${CMAKE_BINARY_DIR}/run_coverage.sh")
        set(COVERAGE_LCOV "${COVERAGE_DIR}/coverage.lcov")
        file(GENERATE OUTPUT ${COVERAGE_SCRIPT} CONTENT "#!/bin/bash
set -e
cd '${CMAKE_BINARY_DIR}'

# Merge profile data
echo 'Merging profile data...'
'${LLVM_PROFDATA_PATH}' merge -sparse -o '${PROFDATA_FILE}' '${COVERAGE_DIR}'/*.profraw

# Generate HTML report
'${LLVM_COV_PATH}' show \\
    ${TEST_OBJECTS_STR} \\
    -instr-profile='${PROFDATA_FILE}' \\
    -format=html \\
    -output-dir='${COVERAGE_HTML}' \\
    -show-line-counts-or-regions \\
    -show-instantiations=false \\
    -ignore-filename-regex='(tests|_deps|catch2|Catch2)/.*'

# Export to lcov format for Codecov
'${LLVM_COV_PATH}' export \\
    ${TEST_OBJECTS_STR} \\
    -instr-profile='${PROFDATA_FILE}' \\
    -format=lcov \\
    -ignore-filename-regex='(tests|_deps|catch2|Catch2)/.*' \\
    > '${COVERAGE_LCOV}'

# Generate summary report
'${LLVM_COV_PATH}' report \\
    ${TEST_OBJECTS_STR} \\
    -instr-profile='${PROFDATA_FILE}' \\
    -ignore-filename-regex='(tests|_deps|catch2|Catch2)/.*'

echo 'Coverage report: ${COVERAGE_HTML}/index.html'
echo 'LCOV export: ${COVERAGE_LCOV}'
")

        add_custom_target(${TARGET_NAME}
            DEPENDS ${ARGN}

            # Clean previous coverage data
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${COVERAGE_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}

            # Run tests with profile output
            COMMAND ${CMAKE_COMMAND} -E env "LLVM_PROFILE_FILE=${COVERAGE_DIR}/%p.profraw"
                ${CMAKE_CTEST_COMMAND} --output-on-failure -j4

            # Run the coverage script
            COMMAND bash ${COVERAGE_SCRIPT}

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating code coverage report..."
        )
    endif()
endfunction()

# Function to add coverage flags to a specific target
function(target_enable_coverage TARGET_NAME)
    if(NOT UNITS_ENABLE_COVERAGE)
        return()
    endif()

    target_compile_options(${TARGET_NAME} PRIVATE ${COVERAGE_COMPILER_FLAGS})
    target_link_options(${TARGET_NAME} PRIVATE ${COVERAGE_LINKER_FLAGS})
endfunction()
