local render_config = {
    render_engine  = "DirectX",
    physics_engine = "BulletPhysics",
    shading_type   = "PBR",
    texture_type   = "PBR",
}

local function map_uv(u, v) return { u = u, v = v } end
local function map_xy(x, y) return { x = x, y = y } end
local function map_xz(x, z) return { x = x, z = z } end

local function print_config()
    print("Render Engine  : " .. render_config.render_engine)
    print("Physics Engine : " .. render_config.physics_engine)
    print("Shading Type   : " .. render_config.shading_type)
    print("Texture Type   : " .. render_config.texture_type)
end

render_config.map_uv      = map_uv
render_config.map_xy      = map_xy
render_config.map_xz      = map_xz
render_config.print_config = print_config

return render_config
