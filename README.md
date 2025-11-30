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

## Development Container

A Docker/Podman development environment is available:

```bash
docker-compose run --rm dev
# or
podman-compose run --rm dev
```

This provides an Alpine-based container with Clang, LLDB, and xmake pre-configured.
