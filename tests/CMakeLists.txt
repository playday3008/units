# Static tests (compile-time verification via static_assert)
# Each file has its own main(), so create separate executables
set(STATIC_TEST_SOURCES
    static/ratio_test.cpp
    static/unit_test.cpp
    static/quantity_spec_test.cpp
    static/quantity_test.cpp
    static/quantity_point_test.cpp
    static/systems_test.cpp
    static/format_test.cpp
    static/math_test.cpp
)

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
        GIT_TAG        v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)

    # Conversion precision tests
    add_executable(runtime_conversion_precision_test
        runtime/conversion_precision_test.cpp
    )
    target_link_libraries(runtime_conversion_precision_test PRIVATE units::units Catch2::Catch2WithMain)
    add_test(NAME runtime_conversion_precision_test COMMAND runtime_conversion_precision_test)

    # Affine quantity (temperature) tests
    add_executable(runtime_affine_quantity_test
        runtime/affine_quantity_test.cpp
    )
    target_link_libraries(runtime_affine_quantity_test PRIVATE units::units Catch2::Catch2WithMain)
    add_test(NAME runtime_affine_quantity_test COMMAND runtime_affine_quantity_test)

    # Format tests
    add_executable(runtime_format_test
        runtime/format_test.cpp
    )
    target_link_libraries(runtime_format_test PRIVATE units::units Catch2::Catch2WithMain)
    add_test(NAME runtime_format_test COMMAND runtime_format_test)

    # Math tests
    add_executable(runtime_math_test
        runtime/math_test.cpp
    )
    target_link_libraries(runtime_math_test PRIVATE units::units Catch2::Catch2WithMain)
    add_test(NAME runtime_math_test COMMAND runtime_math_test)
endif()

# Compile-fail tests
if(UNITS_ENABLE_COMPILE_FAIL_TESTS)
    function(add_compile_fail_test name source)
        add_executable(${name} ${source})
        target_link_libraries(${name} PRIVATE units::units)
        set_target_properties(${name} PROPERTIES
            EXCLUDE_FROM_ALL TRUE
            EXCLUDE_FROM_DEFAULT_BUILD TRUE
        )
        add_test(
            NAME ${name}
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}
        )
        set_tests_properties(${name} PROPERTIES WILL_FAIL TRUE)
    endfunction()

    add_compile_fail_test(fail_add_incompatible_dimensions
        compile_fail/add_incompatible_dimensions.cpp)
    add_compile_fail_test(fail_implicit_narrowing
        compile_fail/implicit_narrowing.cpp)
    add_compile_fail_test(fail_point_plus_point compile_fail/point_plus_point.cpp)
    add_compile_fail_test(fail_torque_to_energy compile_fail/torque_to_energy.cpp)
endif()

# Example tests - verify example code compiles and runs
option(UNITS_ENABLE_EXAMPLE_TESTS "Enable example tests" ON)

if(UNITS_ENABLE_EXAMPLE_TESTS)
    set(EXAMPLE_TEST_SOURCES
        examples/basic_quantities.cpp
        examples/cgs_units.cpp
        examples/derived_quantities.cpp
        examples/natural_units.cpp
        examples/quick_start.cpp
        examples/imperial_units.cpp
        examples/math_functions.cpp
        examples/unit_conversions.cpp
        examples/zero_overhead.cpp
        examples/temperature_conversions.cpp
        examples/math_functions_extended.cpp
        examples/formatting.cpp
    )

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
