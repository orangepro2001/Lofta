#!/usr/bin/env python3
"""Writes the program icon: a Roman XI on a slip of paper, for the eleven lost days. ICO with
PNG images inside, made with the standard library only."""
import struct
import sys
import zlib

INK = (28, 22, 18)
PAPER = (226, 214, 186)
EDGE = (120, 98, 66)
CLEAR = (0, 0, 0)

# 16 x 16 art; scaled up by whole numbers so every size stays sharp.
ART = [
    "................",
    "..############..",
    ".##oooooooooo##.",
    ".#oooooooooooo#.",
    ".#o@@oo@@o@@@o#.",
    ".#oo@oo@oo.@oo#.",
    ".#oo.@@ooo.@oo#.",
    ".#ooo@@ooo.@oo#.",
    ".#ooo@@ooo.@oo#.",
    ".#oo@oo@oo.@oo#.",
    ".#oo@oo@oo.@oo#.",
    ".#o@@oo@@o@@@o#.",
    ".#oooooooooooo#.",
    ".##oooooooooo##.",
    "..############..",
    "................",
]
COLORS = {".": CLEAR, "#": EDGE, "o": PAPER, "@": INK}


def png(size):
    scale = size // 16
    rows = []
    for row in ART:
        line = bytearray([0])
        for ch in row:
            r, g, b = COLORS.get(ch, PAPER)
            alpha = 0 if ch == "." else 255
            line += bytes((r, g, b, alpha)) * scale
        rows.extend([bytes(line)] * scale)

    def chunk(kind, data):
        return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF)

    header = struct.pack(">IIBBBBB", size, size, 8, 6, 0, 0, 0)
    return (b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", header) + chunk(b"IDAT", zlib.compress(b"".join(rows), 9)) +
            chunk(b"IEND", b""))


def main(path):
    sizes = [16, 32, 48, 64, 256]
    images = [png(s) for s in sizes]
    head = struct.pack("<HHH", 0, 1, len(images))
    offset = len(head) + 16 * len(images)
    entries = b""
    for size, data in zip(sizes, images):
        entries += struct.pack("<BBBBHHII", size % 256, size % 256, 0, 0, 1, 32, len(data), offset)
        offset += len(data)
    with open(path, "wb") as f:
        f.write(head + entries + b"".join(images))


if __name__ == "__main__":
    main(sys.argv[1])
