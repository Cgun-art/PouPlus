const RENDER_ENGINE: string  = "DirectX";
const PHYSICS_ENGINE: string = "BulletPhysics";
const SHADING_TYPE: string   = "PBR";
const TEXTURE_TYPE: string   = "PBR";

interface UVMap { u: number; v: number; }
interface XYMap { x: number; y: number; }
interface XZMap { x: number; z: number; }

const mapUV = (u: number, v: number): UVMap => ({ u, v });
const mapXY = (x: number, y: number): XYMap => ({ x, y });
const mapXZ = (x: number, z: number): XZMap => ({ x, z });

function printConfig(): void {
    console.log(`Render Engine  : ${RENDER_ENGINE}`);
    console.log(`Physics Engine : ${PHYSICS_ENGINE}`);
    console.log(`Shading Type   : ${SHADING_TYPE}`);
    console.log(`Texture Type   : ${TEXTURE_TYPE}`);
}

export { RENDER_ENGINE, PHYSICS_ENGINE, SHADING_TYPE, TEXTURE_TYPE, mapUV, mapXY, mapXZ, printConfig };
