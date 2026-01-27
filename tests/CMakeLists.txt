# Static tests (compile-time verification via static_assert)
# Each file has its own main(), so create separate executables
file(GLOB STATIC_TEST_SOURCES CONFIGURE_DEPENDS "static/*.cpp")

foreach(source ${STATIC_TEST_SOURCES})
    get_filename_component(test_name ${source} NAME_WE)
    set(target_name "static_${test_name}")
    add_executable(${target_name} ${source})
    target_link_libraries(${target_name} PRIVATE units::units)
    add_test(NAME ${target_name} COMMAND ${target_name})
endforeach()

# Convenience target to build all static tests
add_custom_target(static_tests)
foreach(source ${STATIC_TEST_SOURCES})
    get_filename_component(test_name ${source} NAME_WE)
    add_dependencies(static_tests "static_${test_name}")
endforeach()

# Runtime tests with Catch2
option(UNITS_ENABLE_RUNTIME_TESTS "Enable runtime tests with Catch2" ON)

if(UNITS_ENABLE_RUNTIME_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.12.0
    )
    FetchContent_MakeAvailable(Catch2)

    file(GLOB RUNTIME_TEST_SOURCES CONFIGURE_DEPENDS "runtime/*.cpp")

    foreach(source ${RUNTIME_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        set(target_name "runtime_${test_name}")
        add_executable(${target_name} ${source})
        target_link_libraries(${target_name} PRIVATE units::units Catch2::Catch2WithMain)
        add_test(NAME ${target_name} COMMAND ${target_name})
    endforeach()

    # Convenience target to build all runtime tests
    add_custom_target(runtime_tests)
    foreach(source ${RUNTIME_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        add_dependencies(runtime_tests "runtime_${test_name}")
    endforeach()
endif()

# Compile-fail tests
if(UNITS_ENABLE_COMPILE_FAIL_TESTS)
    file(GLOB COMPILE_FAIL_SOURCES CONFIGURE_DEPENDS "compile_fail/*.cpp")

    foreach(source ${COMPILE_FAIL_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        set(target_name "fail_${test_name}")
        add_executable(${target_name} ${source})
        target_link_libraries(${target_name} PRIVATE units::units)
        set_target_properties(${target_name} PROPERTIES
            EXCLUDE_FROM_ALL TRUE
            EXCLUDE_FROM_DEFAULT_BUILD TRUE
        )
        add_test(
            NAME ${target_name}
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${target_name}
        )
        set_tests_properties(${target_name} PROPERTIES WILL_FAIL TRUE)
    endforeach()
endif()

# Example tests - verify example code compiles and runs
option(UNITS_ENABLE_EXAMPLE_TESTS "Enable example tests" ON)

if(UNITS_ENABLE_EXAMPLE_TESTS)
    file(GLOB EXAMPLE_TEST_SOURCES CONFIGURE_DEPENDS "examples/*.cpp")

    foreach(source ${EXAMPLE_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        set(target_name "example_${test_name}")
        add_executable(${target_name} ${source})
        target_link_libraries(${target_name} PRIVATE units::units)
        add_test(NAME ${target_name} COMMAND ${target_name})
    endforeach()

    # Convenience target to build all example tests
    add_custom_target(example_tests)
    foreach(source ${EXAMPLE_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        add_dependencies(example_tests "example_${test_name}")
    endforeach()
endif()

# Compile-time benchmark
add_executable(compile_time_benchmark benchmark/compile_time_benchmark.cpp)
target_link_libraries(compile_time_benchmark PRIVATE units::units)
set_target_properties(compile_time_benchmark PROPERTIES
    EXCLUDE_FROM_ALL TRUE
    EXCLUDE_FROM_DEFAULT_BUILD TRUE
)

# Code coverage target
if(UNITS_ENABLE_COVERAGE AND UNITS_ENABLE_RUNTIME_TESTS)
    # Collect all runtime test targets for coverage
    set(COVERAGE_TEST_TARGETS "")
    foreach(source ${RUNTIME_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        list(APPEND COVERAGE_TEST_TARGETS "runtime_${test_name}")
    endforeach()

    # Also include static tests
    foreach(source ${STATIC_TEST_SOURCES})
        get_filename_component(test_name ${source} NAME_WE)
        list(APPEND COVERAGE_TEST_TARGETS "static_${test_name}")
    endforeach()

    add_coverage_target(coverage ${COVERAGE_TEST_TARGETS})
endif()
