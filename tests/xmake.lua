-- tests/xmake.lua

-- Catch2 package for runtime tests
add_requires("catch2 3.x", {alias = "catch2"})

-- Static assertion tests - each file is a separate target
for _, file in ipairs(os.files("static/*.cpp")) do
    local name = "static_" .. path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_deps("units")
        add_files(file)
        set_group("tests/static")
    target_end()
end

-- Convenience target to build all static tests
target("static_tests")
    set_kind("phony")
    set_default(false)
    for _, file in ipairs(os.files("static/*.cpp")) do
        local name = "static_" .. path.basename(file)
        add_deps(name)
    end
    set_group("tests/static")
target_end()

-- Runtime tests with Catch2
for _, file in ipairs(os.files("runtime/*.cpp")) do
    local name = "runtime_" .. path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_deps("units")
        add_packages("catch2")
        add_files(file)
        set_group("tests/runtime")
    target_end()
end

-- Convenience target to build all runtime tests
target("runtime_tests")
    set_kind("phony")
    set_default(false)
    for _, file in ipairs(os.files("runtime/*.cpp")) do
        local name = "runtime_" .. path.basename(file)
        add_deps(name)
    end
    set_group("tests/runtime")
target_end()

-- Compile-fail tests using custom rule
if has_config("compile_fail_tests") then
    rule("compile_fail")
        on_build(function (target)
            -- Compile should fail - we check the return code
            local ok = try {
                function ()
                    import("core.project.project")
                    import("core.tool.compiler")

                    for _, sourcefile in ipairs(target:sourcefiles()) do
                        compiler.compile(sourcefile, target:objectfile(sourcefile), {
                            target = target
                        })
                    end
                    return true
                end
            }

            if ok then
                raise("Compile-fail test %s should have failed to compile!", target:name())
            else
                print("Compile-fail test %s correctly failed to compile", target:name())
            end
        end)

        on_run(function (target)
            -- Already verified at build time
        end)
    rule_end()

    -- Compile-fail test targets
    for _, file in ipairs(os.files("compile_fail/*.cpp")) do
        local name = "fail_" .. path.basename(file)
        target(name)
            set_kind("binary")
            set_default(false)
            add_rules("compile_fail")
            add_deps("units")
            add_files(file)
            set_group("tests/compile_fail")
        target_end()
    end
end

-- Example tests - demonstrates API usage and verifies README examples compile
for _, file in ipairs(os.files("examples/*.cpp")) do
    local name = "example_" .. path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_deps("units")
        add_files(file)
        set_group("tests/examples")
    target_end()
end

-- Convenience target to build all example tests
target("example_tests")
    set_kind("phony")
    set_default(false)
    for _, file in ipairs(os.files("examples/*.cpp")) do
        local name = "example_" .. path.basename(file)
        add_deps(name)
    end
    set_group("tests/examples")
target_end()

-- Compile-time benchmark
target("compile_time_benchmark")
    set_kind("binary")
    set_default(false)
    add_deps("units")
    add_files("benchmark/compile_time_benchmark.cpp")
    set_group("tests/benchmark")
target_end()

-- Tests aggregation target
target("tests")
    set_kind("phony")
    set_default(false)
    add_deps("static_tests")
    add_deps("runtime_tests")

    on_run(function (target)
        import("core.project.project")

        -- Run static tests (just needs to compile and run)
        print("Running static assertion tests...")
        for _, file in ipairs(os.files("static/*.cpp")) do
            local name = "static_" .. path.basename(file)
            local t = project.target(name)
            if t and t:targetfile() then
                os.execv(t:targetfile())
            end
        end

        -- Run runtime tests
        print("Running runtime tests...")
        for _, file in ipairs(os.files("runtime/*.cpp")) do
            local name = "runtime_" .. path.basename(file)
            local t = project.target(name)
            if t and t:targetfile() then
                os.execv(t:targetfile())
            end
        end

        print("All tests passed!")
    end)
target_end()
