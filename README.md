# AbstractProgrammingProject

A C++ project built with xmake.

## Prerequisites

- [xmake](https://xmake.io) build system
- C++ compiler (Clang or GCC)
- Debugger (LLDB or GDB)

## Configure

```bash
xmake f --toolchain=clang -c # Force use Clang as the compiler
xmake f --debugger=lldb -c   # Force use LLDB as the debugger
xmake f -m debug             # Set default build mode to debug
```

## Building

```bash
xmake            # Build in default mode (release)
xmake -m release # Build in release mode
xmake -m debug   # Build in debug mode
```

### Build Modes

| Mode | Description |
|------|-------------|
| `debug` | Debug mode |
| `release` | Release mode |
| `releasedbg` | Release with debug info |
| `minsizerel` | Minimum size release |
| `check` | Sanitizers enabled (address, thread, memory, leak, undefined) |
| `profile` | Profiling mode |
| `coverage` | Code coverage mode |

## Running

```bash
xmake run
```

## Debugging

```bash
# Better be built in debug mode first
# Or good luck debugging release builds!
xmake run -d
```

## Testing

```bash
xmake test -v
```

## Linting

```bash
xmake check clang.tidy
```

## Formatting

```bash
xmake format                 # Format all source files
xmake format -f "src/*.cpp"  # Format specific files
```

## Development Container

A Docker/Podman development environment is available:

```bash
docker-compose run --rm dev
# or
podman-compose run --rm dev
```

This provides an Alpine-based container with Clang, LLDB, and xmake pre-configured.
