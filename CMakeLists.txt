cmake_minimum_required(VERSION 3.28)
project(units
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "C++23 compile-time physical quantities and units library"
)

# Enable compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(UNITS_BUILD_TESTS "Build unit tests" ON)
option(UNITS_ENABLE_COMPILE_FAIL_TESTS "Enable compile-fail tests" ON)
option(UNITS_EMBEDDED_FRIENDLY "Disable exceptions and RTTI" OFF)

# Code coverage support
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

# Header-only library target
add_library(units INTERFACE)
add_library(units::units ALIAS units)

target_include_directories(units INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(units INTERFACE cxx_std_23)

# Embedded-friendly configuration
if(UNITS_EMBEDDED_FRIENDLY)
    target_compile_definitions(units INTERFACE UNITS_NO_EXCEPTIONS UNITS_NO_RTTI)
    target_compile_options(units INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fno-exceptions -fno-rtti>
        $<$<CXX_COMPILER_ID:MSVC>:/EHs-c- /GR->
    )
endif()

# Examples
option(UNITS_BUILD_EXAMPLES "Build examples" ON)
if(UNITS_BUILD_EXAMPLES)
    add_executable(examples src/main.cpp)
    target_link_libraries(examples PRIVATE units::units)
endif()

# Testing
if(UNITS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS units
    EXPORT unitsTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/units
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT unitsTargets
    FILE unitsTargets.cmake
    NAMESPACE units::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/units
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/unitsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/unitsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/units
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/unitsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/unitsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/unitsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/units
)
