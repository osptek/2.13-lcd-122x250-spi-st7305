#!/usr/bin/env python3
"""将 PNG 转为 ST7305 2.13 寸 (122x250) 显存 C 头文件。需 Pillow: pip install pillow"""

import argparse
import sys

W, H = 122, 250
DATA_W = 33
COL_OFF = 10


def pack_image(im) -> bytes:
    from PIL import Image

    im = im.convert("L")
    if im.size != (W, H):
        im = im.resize((W, H), Image.Resampling.LANCZOS)
    pix = im.load()
    buf = bytearray(DATA_W * (H // 2))
    for y in range(H):
        for x in range(W):
            on = 1 if pix[x, y] < 128 else 0
            px = x + COL_OFF
            real_x = px // 4
            real_y = y // 2
            idx = real_y * DATA_W + real_x
            one_two = 0 if (y % 2 == 0) else 1
            line_bit_4 = px % 4
            write_bit = 7 - (line_bit_4 * 2 + one_two)
            if on:
                buf[idx] |= 1 << write_bit
            else:
                buf[idx] &= ~(1 << write_bit)
    return bytes(buf)


def write_header(buf: bytes, out_path: str, sym: str) -> None:
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("#pragma once\n\n")
        f.write(f"// Auto-generated for ST7305 2.13 ({W}x{H} packed)\n\n")
        f.write(f"const int {sym}_w = {W};\n")
        f.write(f"const int {sym}_h = {H};\n")
        f.write(f"const unsigned int {sym}_len = {len(buf)};\n\n")
        f.write(f"const unsigned char {sym}[] = {{\n")
        for i in range(0, len(buf), 16):
            chunk = buf[i : i + 16]
            f.write("    " + ", ".join(f"0x{b:02X}" for b in chunk) + ",\n")
        f.write("};\n")


def main() -> int:
    try:
        from PIL import Image
    except ImportError:
        print("需要 Pillow: python3 -m venv .venv && .venv/bin/pip install pillow", file=sys.stderr)
        return 1

    p = argparse.ArgumentParser(description="PNG -> ST7305 packed .h")
    p.add_argument("png", help="输入 PNG（建议 122x250）")
    p.add_argument("-o", "--output", required=True, help="输出 .h 路径")
    p.add_argument("-n", "--name", default="image", help="C 符号名，默认 image")
    args = p.parse_args()

    im = Image.open(args.png)
    buf = pack_image(im)
    write_header(buf, args.output, args.name)
    print(f"wrote {args.output} ({len(buf)} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
