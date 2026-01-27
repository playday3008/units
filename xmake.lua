-- xmake.lua
set_project("units")
set_version("0.1.0")
set_xmakever("2.8.0")

add_rules(
    "mode.debug",
    "mode.release",
    "mode.releasedbg",
    "mode.check"
)

-- Custom coverage mode (don't use mode.coverage - it's GCC-specific)
if is_mode("coverage") then
    set_symbols("debug")
    set_optimize("none")
end

-- Generate compile_commands.json for IDE integration
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

-- C++23 requirement
set_languages("c++23")

-- Enable all, extra, and pedantic warnings
set_warnings("all", "extra", "pedantic")

-- Enable sanitizers in check mode
if is_mode("check") then
    set_policy("build.sanitizer.address", true)
    set_policy("build.sanitizer.undefined", true)
end

-- Add coverage rule for toolchain-specific flags
rule("coverage_flags")
    on_config(function (target)
        if not is_mode("coverage") then
            return
        end
        -- Check tool name: clangxx for Clang, gxx for GCC
        if target:has_tool("cxx", "clangxx") then
            -- Clang: source-based coverage (llvm-cov)
            target:add("cxflags", "-fprofile-instr-generate", "-fcoverage-mapping", {force = true})
            target:add("ldflags", "-fprofile-instr-generate", {force = true})
            target:add("shflags", "-fprofile-instr-generate", {force = true})
            -- Workaround for Fedora/RHEL where runtime libs use distro-specific target triple
            if is_host("linux") then
                local target_triple = try { function() return os.iorunv("clang", {"--version"}):match("Target: ([^\n]+)") end }
                if target_triple and target_triple:find("redhat") then
                    target:add("cxflags", "--target=" .. target_triple, {force = true})
                    target:add("ldflags", "--target=" .. target_triple, {force = true})
                end
            end
        elseif target:has_tool("cxx", "gxx") then
            -- GCC: gcov-based coverage
            target:add("cxflags", "--coverage", "-fprofile-arcs", "-ftest-coverage", {force = true})
            target:add("ldflags", "--coverage", {force = true})
            target:add("shflags", "--coverage", {force = true})
        end
    end)
rule_end()

-- Apply coverage rule globally
add_rules("coverage_flags")

-- Options
option("tests", {default = true, description = "Build unit tests"})
option("runtime_tests", {default = true, description = "Enable runtime tests with Catch2"})
option("compile_fail_tests", {default = true, description = "Enable compile-fail tests"})
option("embedded", {default = false, description = "Embedded-friendly (no exceptions/RTTI)"})

-- Main header-only library
target("units")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(units/**.hpp)")

    -- Embedded configuration
    if has_config("embedded") then
        add_defines("UNITS_NO_EXCEPTIONS", "UNITS_NO_RTTI", {public = true})
        add_cxxflags("-fno-exceptions", "-fno-rtti", {tools = {"gcc", "clang"}})
        add_cxxflags("/EHs-c-", "/GR-", {tools = {"cl"}})
    end
target_end()

-- Examples
target("examples")
    set_kind("binary")
    set_default(true)
    add_deps("units")
    add_files("src/main.cpp")
    set_group("examples")
target_end()

-- Include test configuration
if has_config("tests") then
    includes("tests")
end

-- Installation rules
on_install(function (target)
    os.cp("include/units", path.join(target:installdir(), "include", "units"))
end)

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

-- Define a custom task for code coverage
task("coverage")
    on_run(function ()
        import("core.base.option")
        import("core.project.project")
        import("core.project.config")
        import("lib.detect.find_tool")

        -- Load project config (required in task context)
        config.load()

        local outputdir = option.get("outputdir") or "build/coverage"

        -- Verify we're in coverage mode
        local mode = config.get("mode")
        if mode ~= "coverage" then
            raise("Coverage mode required! Run: xmake f -m coverage --toolchain=clang -y && xmake coverage")
        end

        -- Detect toolchain - be explicit about what compiler is being used
        local toolchain = config.get("toolchain") or ""
        local is_clang = toolchain:find("clang") or toolchain:find("llvm")
        local is_gcc = toolchain:find("gcc") or (toolchain == "" and not is_clang)

        -- Find coverage tools
        local lcov = find_tool("lcov")
        local genhtml = find_tool("genhtml")
        local llvm_cov = find_tool("llvm-cov")
        local llvm_profdata = find_tool("llvm-profdata")

        -- Use LLVM tools for Clang, lcov/gcov for GCC (don't auto-fallback)
        local use_llvm = is_clang

        if use_llvm then
            if not llvm_cov or not llvm_profdata then
                raise("llvm-cov and llvm-profdata required for Clang coverage! Install LLVM tools.")
            end
        else
            if not lcov or not genhtml then
                raise("lcov and genhtml required for GCC coverage! Install with: sudo dnf install lcov")
            end
        end

        print("Toolchain: " .. (is_clang and "Clang" or "GCC"))

        -- Get build directory using xmake's config (platform-independent)
        local build_dir = path.join(config.builddir(), config.plat(), config.arch(), config.mode())
        print("Build directory: " .. build_dir)

        -- Collect test targets and their executables
        local function collect_test_executables()
            local executables = {}
            for _, file in ipairs(os.files("tests/runtime/*.cpp")) do
                local name = "runtime_" .. path.basename(file)
                local t = project.target(name)
                if t and t:targetfile() then
                    table.insert(executables, {name = name, path = t:targetfile()})
                end
            end
            for _, file in ipairs(os.files("tests/static/*.cpp")) do
                local name = "static_" .. path.basename(file)
                local t = project.target(name)
                if t and t:targetfile() then
                    table.insert(executables, {name = name, path = t:targetfile()})
                end
            end
            return executables
        end

        -- Run tests and collect results
        local function run_tests(executables)
            local passed, failed = 0, 0
            for _, test in ipairs(executables) do
                local ok = try { function()
                    os.execv(test.path)
                    return true
                end }
                if ok then
                    passed = passed + 1
                else
                    failed = failed + 1
                    print("FAILED: " .. test.name)
                end
            end
            print(string.format("Tests: %d passed, %d failed", passed, failed))
            return failed == 0
        end

        -- Create output directory
        local abs_outputdir = path.absolute(outputdir)
        os.mkdir(abs_outputdir)

        -- Collect test executables
        local executables = collect_test_executables()
        if #executables == 0 then
            raise("No test executables found! Build tests first: xmake build runtime_tests static_tests")
        end
        print(string.format("Found %d test executables", #executables))

        if use_llvm then
            -- LLVM coverage workflow
            print("Using llvm-cov for coverage...")

            -- Set profile output path before running tests
            os.setenv("LLVM_PROFILE_FILE", path.join(abs_outputdir, "%p.profraw"))

            -- Run tests
            print("Running tests...")
            run_tests(executables)

            -- Merge profile data
            local profraw_files = os.files(path.join(abs_outputdir, "*.profraw"))
            if #profraw_files == 0 then
                raise("No profile data generated! Tests may have failed to run.")
            end

            local profdata = path.join(abs_outputdir, "coverage.profdata")
            print("Merging " .. #profraw_files .. " profile files...")
            os.execv(llvm_profdata.program, {"merge", "-sparse", "-o", profdata, table.unpack(profraw_files)})

            -- Build object list for llvm-cov (include both runtime and static tests)
            local objects = {}
            for _, test in ipairs(executables) do
                if os.isfile(test.path) then
                    table.insert(objects, "-object=" .. path.absolute(test.path))
                end
            end

            if #objects == 0 then
                raise("No test executables found for coverage report!")
            end

            -- Generate HTML report
            local html_dir = path.join(abs_outputdir, "html")
            os.mkdir(html_dir)

            local show_args = {"show"}
            for _, obj in ipairs(objects) do table.insert(show_args, obj) end
            table.insert(show_args, "-instr-profile=" .. profdata)
            table.insert(show_args, "-format=html")
            table.insert(show_args, "-output-dir=" .. html_dir)
            table.insert(show_args, "-show-line-counts-or-regions")
            table.insert(show_args, "-ignore-filename-regex=tests/.*")
            os.execv(llvm_cov.program, show_args)

            -- Generate summary report
            local report_args = {"report"}
            for _, obj in ipairs(objects) do table.insert(report_args, obj) end
            table.insert(report_args, "-instr-profile=" .. profdata)
            table.insert(report_args, "-ignore-filename-regex=tests/.*")
            os.execv(llvm_cov.program, report_args)

            print("Coverage report: " .. path.join(html_dir, "index.html"))
        else
            -- lcov/gcov workflow
            print("Using lcov/gcov for coverage...")

            local coverage_info = path.join(abs_outputdir, "coverage.info")
            local html_dir = path.join(abs_outputdir, "html")

            -- The .gcno/.gcda files are in build/.objs/, so use root build directory
            local gcov_dir = path.absolute(config.builddir())
            print("Scanning for coverage data in: " .. gcov_dir)

            -- Reset coverage counters
            os.execv(lcov.program, {"--zerocounters", "--directory", gcov_dir})

            -- Capture baseline
            os.execv(lcov.program, {"--capture", "--initial",
                "--directory", gcov_dir,
                "--output-file", path.join(abs_outputdir, "baseline.info"),
                "--ignore-errors", "mismatch,source,empty",
                "--rc", "geninfo_unexecuted_blocks=1"
            })

            -- Run tests
            print("Running tests...")
            run_tests(executables)

            -- Capture test coverage
            os.execv(lcov.program, {"--capture",
                "--directory", gcov_dir,
                "--output-file", path.join(abs_outputdir, "test.info"),
                "--ignore-errors", "mismatch,source,empty",
                "--rc", "geninfo_unexecuted_blocks=1"
            })

            -- Combine baseline and test coverage
            os.execv(lcov.program, {
                "--add-tracefile", path.join(abs_outputdir, "baseline.info"),
                "--add-tracefile", path.join(abs_outputdir, "test.info"),
                "--output-file", coverage_info,
                "--ignore-errors", "mismatch,source"
            })

            -- Remove system headers and test code
            os.execv(lcov.program, {"--remove", coverage_info,
                "/usr/*",
                "*/catch2/*",
                "*/tests/*",
                "--output-file", coverage_info,
                "--ignore-errors", "mismatch,source,unused"
            })

            -- Generate HTML report
            os.mkdir(html_dir)
            os.execv(genhtml.program, {coverage_info,
                "--output-directory", html_dir,
                "--title", "Units Library Coverage",
                "--legend",
                "--show-details",
                "--demangle-cpp"
            })

            print("Coverage report: " .. path.join(html_dir, "index.html"))
        end
    end)

    set_menu {
        usage = "xmake coverage [options]",
        description = "Generate code coverage report (requires: xmake f -m coverage)",
        options = {
            {'o', "outputdir", "kv", "build/coverage", "Output directory for coverage report"}
        }
    }

-- Define a custom task for clang-tidy linting (excludes compile_fail tests)
task("lint")
    on_run(function ()
        import("core.base.option")

        -- Collect all source files, excluding compile_fail directory
        local files = {}
        for _, f in ipairs(os.files("include/**.hpp")) do
            table.insert(files, f)
        end
        for _, f in ipairs(os.files("src/**.cpp")) do
            table.insert(files, f)
        end
        for _, f in ipairs(os.files("tests/**.cpp")) do
            -- Exclude compile_fail directory (intentional compile errors)
            if not f:find("compile_fail", 1, true) then
                table.insert(files, f)
            end
        end

        -- Build file pattern string for clang-tidy
        local pattern = table.concat(files, ":")

        -- Build command arguments, forwarding all options
        local args = {"check", "clang.tidy", "-f", pattern}
        if option.get("fix") then table.insert(args, "--fix") end
        if option.get("fix_errors") then table.insert(args, "--fix_errors") end
        if option.get("fix_notes") then table.insert(args, "--fix_notes") end
        if option.get("quiet") then table.insert(args, "--quiet") end
        if option.get("list") then table.insert(args, "--list") end
        if option.get("create") then table.insert(args, "--create") end
        if option.get("jobs") then table.insert(args, "-j") table.insert(args, option.get("jobs")) end
        if option.get("checks") then table.insert(args, "--checks=" .. option.get("checks")) end
        if option.get("configfile") then table.insert(args, "--configfile=" .. option.get("configfile")) end

        os.execv("xmake", args)
    end)

    set_menu {
        usage = "xmake lint [options]",
        description = "Run clang-tidy (excludes compile_fail tests)",
        options = {
            {nil, "fix", "k", nil, "Apply suggested fixes"},
            {nil, "fix_errors", "k", nil, "Apply suggested error fixes"},
            {nil, "fix_notes", "k", nil, "Apply suggested note fixes"},
            {'l', "list", "k", nil, "Show clang-tidy checks list"},
            {nil, "create", "k", nil, "Create a .clang-tidy file"},
            {'j', "jobs", "kv", nil, "Number of parallel jobs"},
            {nil, "checks", "kv", nil, "Set the given checks"},
            {nil, "configfile", "kv", nil, "Path to .clang-tidy config file"}
        }
    }

-- Package definition using xpack
includes("@builtin/xpack")

xpack("units")
    --set_homepage("https://github.com/...") -- TODO: Update homepage URL
    set_description("C++23 compile-time physical quantities and units library")
    set_license("MIT")
    set_formats("srczip", "srctargz")

    add_sourcefiles("include/(units/**.hpp)")
    add_sourcefiles("LICENSE")
    add_sourcefiles("README.md")

    on_load(function (package)
        package:set("basename", "units-v" .. package:version())
    end)
xpack_end()
