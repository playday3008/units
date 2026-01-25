-- xmake.lua
set_project("units")
set_version("0.1.0")
set_xmakever("2.8.0")

add_rules(
    "mode.debug",
    "mode.release",
    "mode.releasedbg",
    "mode.check",
    "mode.coverage"
)

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

-- Options
option("tests", {default = true, description = "Build unit tests"})
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
