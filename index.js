import { createRequire } from 'module';
import { createWriteStream } from 'fs';
import zlib from 'zlib';

const require = createRequire(import.meta.url);
const { exec } = require('child_process');

console.log('Shiddin, Aho!, pwamp.');

// --- Shared PNG utilities ---

const CRC_TABLE = new Uint32Array(256);
for (let i = 0; i < 256; i++) {
  let c = i;
  for (let k = 0; k < 8; k++) c = (c & 1) ? (0xedb88320 ^ (c >>> 1)) : (c >>> 1);
  CRC_TABLE[i] = c;
}

function crc32(buf) {
  let c = 0xffffffff;
  for (const b of buf) c = CRC_TABLE[(c ^ b) & 0xff] ^ (c >>> 8);
  return (c ^ 0xffffffff) >>> 0;
}

function chunk(type, data) {
  const typeBuf = Buffer.from(type);
  const dataBuf = Buffer.isBuffer(data) ? data : Buffer.from(data);
  const len = Buffer.alloc(4); len.writeUInt32BE(dataBuf.length);
  const crcBuf = Buffer.alloc(4);
  crcBuf.writeUInt32BE(crc32(Buffer.concat([typeBuf, dataBuf])));
  return Buffer.concat([len, typeBuf, dataBuf, crcBuf]);
}

function writePNG(path, ihdr, raw) {
  return new Promise((resolve, reject) => {
    const idat = zlib.deflateSync(Buffer.from(raw));
    const out = createWriteStream(path);
    out.write(Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]));
    out.write(chunk('IHDR', ihdr));
    out.write(chunk('IDAT', idat));
    out.write(chunk('IEND', Buffer.alloc(0)));
    out.end(resolve);
    out.on('error', reject);
  });
}

// --- LUT method: horizontal gradient between two colours ---

function buildLUTGradient(W, H, from, to) {
  function lerp(a, b, t) { return a + (b - a) * t; }
  const raw = [];
  for (let y = 0; y < H; y++) {
    raw.push(0);
    for (let x = 0; x < W; x++) {
      const t = x / (W - 1);
      raw.push(Math.round(lerp(from[0], to[0], t) * 255));
      raw.push(Math.round(lerp(from[1], to[1], t) * 255));
      raw.push(Math.round(lerp(from[2], to[2], t) * 255));
      raw.push(255);
    }
  }
  return raw;
}

// --- PBR method: solid fill --- different from LUT gradient ---

function buildSolidFill(W, H, r, g, b, a = 255) {
  const raw = [];
  for (let y = 0; y < H; y++) {
    raw.push(0);
    for (let x = 0; x < W; x++) {
      raw.push(r, g, b, a);
    }
  }
  return raw;
}

function buildIHDR(W, H) {
  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(W, 0); ihdr.writeUInt32BE(H, 4);
  ihdr[8] = 8; ihdr[9] = 6; ihdr[10] = 0; ihdr[11] = 0; ihdr[12] = 0;
  return ihdr;
}

function norm(v) { return Math.round(v * 255); }

// --- Generate LUT PNG ---

{
  const W = 1024, H = 64;
  const ihdr = buildIHDR(W, H);
  const raw = buildLUTGradient(W, H, [0.580, 0.0, 0.0], [1.0, 1.0, 1.0]);
  await writePNG('luts/color_lut_1024_img.png', ihdr, raw);
  console.log('Generated: luts/color_lut_1024_img.png');
}

// --- Generate Gold PBR maps (256x256 solid fills) ---

const S = 256;
const goldIHDR = buildIHDR(S, S);

await writePNG('materials/gold_albedo.png',   goldIHDR, buildSolidFill(S, S, norm(1.0),   norm(0.766), norm(0.336)));
console.log('Generated: materials/gold_albedo.png');

await writePNG('materials/gold_roughness.png', goldIHDR, buildSolidFill(S, S, norm(0.1),  norm(0.1),   norm(0.1)));
console.log('Generated: materials/gold_roughness.png');

await writePNG('materials/gold_metallic.png',  goldIHDR, buildSolidFill(S, S, norm(1.0),  norm(1.0),   norm(1.0)));
console.log('Generated: materials/gold_metallic.png');

await writePNG('materials/gold_normal.png',    goldIHDR, buildSolidFill(S, S, 128, 128, 255));
console.log('Generated: materials/gold_normal.png');

await writePNG('materials/gold_emission.png',  goldIHDR, buildSolidFill(S, S, 0, 0, 0));
console.log('Generated: materials/gold_emission.png');

// --- Generate Wood PBR maps (256x256 solid fills) ---

const woodIHDR = buildIHDR(S, S);

await writePNG('materials/wood_albedo.png',   woodIHDR, buildSolidFill(S, S, norm(0.588), norm(0.392), norm(0.196)));
console.log('Generated: materials/wood_albedo.png');

await writePNG('materials/wood_roughness.png', woodIHDR, buildSolidFill(S, S, norm(0.75),  norm(0.75),  norm(0.75)));
console.log('Generated: materials/wood_roughness.png');

await writePNG('materials/wood_metallic.png',  woodIHDR, buildSolidFill(S, S, 0, 0, 0));
console.log('Generated: materials/wood_metallic.png');

await writePNG('materials/wood_normal.png',    woodIHDR, buildSolidFill(S, S, 128, 128, 255));
console.log('Generated: materials/wood_normal.png');

await writePNG('materials/wood_emission.png',  woodIHDR, buildSolidFill(S, S, 0, 0, 0));
console.log('Generated: materials/wood_emission.png');
