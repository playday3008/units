add_rules(
    -- Normal build modes
    "mode.debug",       -- Debug mode
    "mode.release",     -- Release mode
    "mode.releasedbg",  -- Release with debug info
    "mode.minsizerel",  -- Minimum size release
    -- Special build modes
    "mode.check",       -- Sanitizers enabled (address, thread, memory, leak, undefined)
    "mode.profile",     -- Profiling mode
    "mode.coverage"     -- Code coverage mode
)

-- Generate compile_commands.json for IDE integration
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

-- Set C++ language standard to C++23
set_languages("c++23")

-- Enable all, extra, and pedantic warnings
set_warnings("all", "extra", "pedantic")

-- Enable C++ exception handling on MSVC
if is_plat("windows") and is_plat("msvc") then
    add_cxflags("/EHsc")
end

-- Add Catch2 package for testing
add_requires("catch2 3.x")

-- Enable sanitizers in check mode
if is_mode("check") then
    set_policy("build.sanitizer.address", true)
    set_policy("build.sanitizer.thread", true)
    set_policy("build.sanitizer.memory", true)
    set_policy("build.sanitizer.leak", true)
    set_policy("build.sanitizer.undefined", true)
end

-- Header-only library
target("example")
    set_kind("headeronly")
    add_headerfiles("include/(example/**.hpp)")
    add_includedirs("include", {public = true})

-- Main executable
target("AbstractProgrammingProject")
    set_kind("binary")
    add_files("src/*.cpp")
    add_deps("example")

-- Test executable
target("tests")
    set_kind("binary")
    set_default(false)
    add_deps("example")
    -- Use dynamic runtime on Windows to match pre-built packages
    on_load(function (target)
        if target:is_plat("windows") then
            if is_mode("debug") or is_mode("releasedbg") then
                target:set("runtimes", "MDd")
            else
                target:set("runtimes", "MD")
            end
        end
    end)
    -- Automatically add test files matching the pattern tests/test_*.cpp
    for _, file in ipairs(os.files("tests/test_*.cpp")) do
        local name = string.replace(path.basename(file), "test_", "")
        add_tests(name, {
            files = file,
            realtime_output = true,
            packages = "catch2",
            runargs = "-s",
        })
    end

-- Define a custom task for code formatting using clang-format
task("format")
    on_run(function ()
        import("core.base.option")
        import("lib.detect.find_tool")
        
        local tool = find_tool("clang-format")
        if not tool then
            raise("clang-format not found!")
        end

        local files = option.get("files") or {}

        -- Collect all source and header files if no files are specified
        if #files == 0 then
            for _, f in ipairs(os.files("include/**.hpp")) do
                table.insert(files, f)
            end
            for _, f in ipairs(os.files("src/**.cpp")) do
                table.insert(files, f)
            end
            for _, f in ipairs(os.files("tests/**.cpp")) do
                table.insert(files, f)
            end
        end

        for _, pattern in ipairs(files) do
            local filelist = os.files(pattern)
            for _, file in ipairs(filelist) do
                os.execv(tool.program, {"-i", file})
                print("Formatting file:", file)
            end
        end
    end)
    
    set_menu {
        usage = "xmake format [options]",
        description = "Format code files",
        options = {
            {'f', "files", "vs", nil, "File patterns to format"}
        }
    }


-- ==============================
-- Basic Build Commands Reference
-- ==============================
--
-- Build modes:
--  xmake f -m <mode>
--
-- Build targets:
--  xmake build [target (optional)]
--
-- Run targets:
--  xmake run [target (optional)]
--
-- Debug targets:
--  xmake run -d [target (optional)]
--
-- Test targets:
--  xmake test -v [target (optional)]
--
-- Linter:
--  xmake check clang.tidy [target (optional)]
--
-- Formatter:
--  xmake format [-f file_patterns (optional)]
--
