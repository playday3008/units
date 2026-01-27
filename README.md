# units - A C++23 Physical Quantities Library

A compile-time dimensional analysis library for C++23, optimized for zero overhead and embedded systems compatibility. Header-only with optional C++23 modules support.

## Quick Start

[📄 tests/examples/quick_start.cpp](./tests/examples/quick_start.cpp)

## Features

- **Zero overhead**: `sizeof(quantity<metre, double>) == sizeof(double)`
- **Compile-time type safety**: Dimension errors caught at compile time
- **Full SI system**: All 7 base units, 22 named derived units, 24 prefixes
- **Multiple unit systems**: CGS, Imperial, Natural units (with literals like `_GeV`, `_MeVc2`, `_fb`)
- **Affine quantities**: Proper handling of temperatures, timestamps
- **Physical constants**: Speed of light, Planck constant, etc.
- **std::format support**: Full formatting with precision and unit symbols
- **Mathematical functions**: abs, sqrt, pow, trig functions
- **C++23 features**: Deducing `this`, `if consteval`, static `operator()`

## Requirements

- **C++23 compiler**: Clang 20+, GCC 14+, or MSVC 19.36+
- **Build system**: xmake (primary) or CMake 3.28+

## Installation

### Header-only (recommended)

Simply copy the `include/units/` directory to your project and add it to your include path.

### xmake (local package)

Add the library as a local package in your project:

```lua
-- In your xmake.lua
add_repositories("local-repo path/to/units")
add_requires("units")

target("myapp")
    add_packages("units")
```

Or use `xmake package` to create a distributable archive.

### CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(
    units
    GIT_REPOSITORY https://github.com/... # TODO
    GIT_TAG main
)
FetchContent_MakeAvailable(units)

target_link_libraries(myapp PRIVATE units::units)
```

Or install locally and use `find_package(units REQUIRED)`.

## Building from Source

### Using xmake

```bash
xmake                           # Build (release mode)
xmake f -m debug && xmake       # Debug build
xmake f -m check && xmake       # Build with sanitizers
xmake build static_tests        # Build tests
xmake run static_tests          # Run tests
```

### Using CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

## Usage Examples

All examples are compiled and tested as part of the build.

| Example                                                                        | Description                                      |
|--------------------------------------------------------------------------------|--------------------------------------------------|
| [📄 basic_quantities.cpp](./tests/examples/basic_quantities.cpp)               | SI literals (`_m`, `_kg`, `_s`, `_N`, `_J`)      |
| [📄 unit_conversions.cpp](./tests/examples/unit_conversions.cpp)               | `.in()` and `.numerical_value_in()` methods      |
| [📄 derived_quantities.cpp](./tests/examples/derived_quantities.cpp)           | Velocity, force, energy calculations             |
| [📄 math_functions.cpp](./tests/examples/math_functions.cpp)                   | `abs`, `sin`, `cos` and other math functions     |
| [📄 formatting.cpp](./tests/examples/formatting.cpp)                           | `std::format` support with precision             |
| [📄 imperial_units.cpp](./tests/examples/imperial_units.cpp)                   | Imperial units and SI conversions                |
| [📄 cgs_units.cpp](./tests/examples/cgs_units.cpp)                             | CGS units (dyne, erg)                            |
| [📄 natural_units.cpp](./tests/examples/natural_units.cpp)                     | Particle physics units (`_GeV`, `_MeVc2`, `_fb`) |
| [📄 temperature_conversions.cpp](./tests/examples/temperature_conversions.cpp) | Kelvin, Celsius, Fahrenheit conversions          |
| [📄 zero_overhead.cpp](./tests/examples/zero_overhead.cpp)                     | Zero-overhead abstraction verification           |
| [📄 math_functions_extended.cpp](./tests/examples/math_functions_extended.cpp) | Extended math (floor, ceil, round, trig)         |

For comprehensive usage, see [📄 src/main.cpp](./src/main.cpp).

## Architecture

The library follows a five-layer type hierarchy:

```chart
Dimension -> QuantitySpec -> Unit -> Reference -> Quantity/QuantityPoint
```

- **Dimension**: Mathematical representation (M, L, T, etc.)
- **QuantitySpec**: Semantic meaning (length, width, speed)
- **Unit**: Symbol and magnitude (metre, kilometre, mile)
- **Reference**: Combines QuantitySpec with Unit
- **Quantity**: Value + Reference (the user-facing type)

## Directory Structure

```tree
include/units/
├── core/
│   ├── ratio.hpp           # Compile-time rationals
│   ├── quantity_spec.hpp   # Quantity specifications
│   ├── unit.hpp            # Unit definitions
│   ├── reference.hpp       # Reference type
│   ├── quantity.hpp        # Main quantity class
│   └── quantity_point.hpp  # Affine quantities
├── systems/
│   ├── isq/                # ISO 80000 quantities
│   ├── si/                 # SI units
│   ├── cgs.hpp             # CGS system
│   ├── imperial.hpp        # Imperial units
│   └── natural.hpp         # Natural units
├── format.hpp              # std::format support
├── math.hpp                # Math functions
└── units.hpp               # Convenience header
```

## Development

### Running Tests

Static tests use compile-time `static_assert`. If it compiles, it passes.

```bash
xmake build static_tests       # Build static assertion tests
xmake run static_tests         # Run static tests
xmake build example_tests      # Build API example tests
xmake run example_basic_quantities  # Run specific example
```

### Code Quality

```bash
xmake lint    # Run linter
xmake format  # Format code
```

### Development Container

```bash
docker-compose run --rm dev
```

## License

MIT License - see LICENSE file for details.

## Acknowledgments

Design inspired by:

- [mp-units](https://github.com/mpusz/mp-units) - The reference C++ units library
- [P2980](https://wg21.link/p2980) - C++ standardization proposal for units
