import math

RENDER_ENGINE  = "DirectX"
PHYSICS_ENGINE = "BulletPhysics"
SHADING_TYPE   = "PBR"
TEXTURE_TYPE   = "PBR"

def map_uv(u: float, v: float) -> dict:
    return {"u": u, "v": v}

def map_xy(x: float, y: float) -> dict:
    return {"x": x, "y": y}

def map_xz(x: float, z: float) -> dict:
    return {"x": x, "z": z}

def print_config():
    print(f"Render Engine  : {RENDER_ENGINE}")
    print(f"Physics Engine : {PHYSICS_ENGINE}")
    print(f"Shading Type   : {SHADING_TYPE}")
    print(f"Texture Type   : {TEXTURE_TYPE}")
