const RENDER_ENGINE  = "DirectX";
const PHYSICS_ENGINE = "BulletPhysics";
const SHADING_TYPE   = "PBR";
const TEXTURE_TYPE   = "PBR";

const mapUV = (u, v) => ({ u, v });
const mapXY = (x, y) => ({ x, y });
const mapXZ = (x, z) => ({ x, z });

function printConfig() {
    console.log(`Render Engine  : ${RENDER_ENGINE}`);
    console.log(`Physics Engine : ${PHYSICS_ENGINE}`);
    console.log(`Shading Type   : ${SHADING_TYPE}`);
    console.log(`Texture Type   : ${TEXTURE_TYPE}`);
}

export { RENDER_ENGINE, PHYSICS_ENGINE, SHADING_TYPE, TEXTURE_TYPE, mapUV, mapXY, mapXZ, printConfig };
