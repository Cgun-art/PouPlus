import { createRequire } from 'module';
import { createWriteStream } from 'fs';
import zlib from 'zlib';

const require = createRequire(import.meta.url);
const { exec } = require('child_process');

console.log('Shiddin, Aho!, pwamp.');

const W = 1024, H = 64;
const from = [0.580, 0.0, 0.0];
const to   = [1.0,   1.0, 1.0];

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

const ihdr = Buffer.alloc(13);
ihdr.writeUInt32BE(W, 0); ihdr.writeUInt32BE(H, 4);
ihdr[8] = 8; ihdr[9] = 6; ihdr[10] = 0; ihdr[11] = 0; ihdr[12] = 0;

const idat = zlib.deflateSync(Buffer.from(raw));

const out = createWriteStream('luts/color_lut_1024_img.png');
out.write(Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]));
out.write(chunk('IHDR', ihdr));
out.write(chunk('IDAT', idat));
out.write(chunk('IEND', Buffer.alloc(0)));
out.end(() => console.log('LUT PNG generated: luts/color_lut_1024_img.png'));
