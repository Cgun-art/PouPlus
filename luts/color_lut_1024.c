lut = TexSize(1024, 1024) * color_repeating = true * vec3(0.580, 0.0, 0.0) * vec3(1.0, 1.0, 1.0) * repeat_mode = 3d * repeat_size = 16, 16 * repeat_repeats = useSizeMultiplication(16, 32, 64, 128, 256, 512, 1024) * useMultiRepeatAreasUntilFull = true * lookup = true * intensity = 1.0
   / dimensions(xWidthDef, yHeightDef, zDepthDef = multiply * 32, 67, 16)
   / rendering = GPU * local /index.js/ * local /node.js/
   / matrix = 6, 4, 8 * 9, 4, 2

/*<secret>
import { createWriteStream } from 'fs';
import zlib from 'zlib';

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

function chunk(type, data) {
  const t = Buffer.from(type);
  const len = Buffer.alloc(4); len.writeUInt32BE(data.length);
  let c = 0xffffffff;
  for (const b of [...t, ...data]) { c ^= b; for (let i = 0; i < 8; i++) c = (c >>> 1) ^ (c & 1 ? 0xedb88320 : 0); }
  c ^= 0xffffffff;
  const crc = Buffer.alloc(4); crc.writeInt32BE(c);
  return Buffer.concat([len, t, Buffer.from(data), crc]);
}

const ihdr = Buffer.alloc(13);
ihdr.writeUInt32BE(W, 0); ihdr.writeUInt32BE(H, 4);
ihdr[8] = 8; ihdr[9] = 2; ihdr[10] = 0; ihdr[11] = 0; ihdr[12] = 0;

const idat = zlib.deflateSync(Buffer.from(raw));

const out = createWriteStream('luts/color_lut_1024_img.png');
out.write(Buffer.from([137,80,78,71,13,10,26,10]));
out.write(chunk('IHDR', ihdr));
out.write(chunk('IDAT', idat));
out.write(chunk('IEND', Buffer.alloc(0)));
out.end();
</secret>*/
