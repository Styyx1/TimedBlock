-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("extern/styyx-util")

-- set project
set_project("simple-timed-block")
set_version("2.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")
set_defaultmode("releasedbg")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set configs
set_config("skyrim_ae", true)
set_config("rex_toml", true)

-- add requires

add_requires("spdlog", { configs = { header_only = false } })

-- targets
target("simple-timed-block")
    -- add dependencies to target
    add_deps("commonlibsse")
    add_deps("styyx-util")
    add_packages("fmt", "spdlog")
    if has_config("skyrim_ae") then
        set_targetdir("/build/SkyrimAE/skse/plugins")
    else
        set_targetdir("/build/SkyrimSE/skse/plugins")
    end  

    -- add commonlibsse plugin
add_rules("commonlibsse.plugin", {
        name = "simple-timed-block",
        author = "styyx",
        description = "Simple timed block mechanic"
})
-- add src files
add_files("src/**.cpp")
add_headerfiles("src/**.h")
add_includedirs("src")
set_pcxxheader("src/pch.h")
add_includedirs("extern/clib-util/include", {public = true})

