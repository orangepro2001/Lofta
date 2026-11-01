#!/usr/bin/env python3
"""Strict PNG validator using only the standard library."""

import sys
import struct
import zlib


class PngError(Exception):
    pass


SIGNATURE = b"\x89PNG\r\n\x1a\n"


def read_chunks(data):
    if len(data) < 8 or data[:8] != SIGNATURE:
        raise PngError("bad signature")
    pos = 8
    chunks = []
    while True:
        if pos + 8 > len(data):
            raise PngError("truncated chunk header")
        length, ctype = struct.unpack(">I4s", data[pos:pos + 8])
        pos += 8
        if pos + length + 4 > len(data):
            raise PngError("truncated chunk data")
        cdata = data[pos:pos + length]
        pos += length
        (crc,) = struct.unpack(">I", data[pos:pos + 4])
        pos += 4
        expected = zlib.crc32(ctype + cdata) & 0xFFFFFFFF
        if crc != expected:
            raise PngError("bad CRC in %s" % ctype.decode("latin-1"))
        chunks.append((ctype, cdata))
        if ctype == b"IEND":
            break
    if pos != len(data):
        raise PngError("trailing bytes after IEND")
    return chunks


def validate(path):
    try:
        with open(path, "rb") as f:
            data = f.read()
    except OSError as e:
        raise PngError("cannot read file: %s" % e.strerror)

    chunks = read_chunks(data)

    if not chunks or chunks[0][0] != b"IHDR":
        raise PngError("IHDR not first")
    ihdr_count = sum(1 for t, _ in chunks if t == b"IHDR")
    if ihdr_count != 1:
        raise PngError("IHDR count != 1")
    ihdr = chunks[0][1]
    if len(ihdr) != 13:
        raise PngError("IHDR length != 13")
    width, height, depth, color, comp, filt, interlace = struct.unpack(">IIBBBBB", ihdr)
    if width == 0 or height == 0:
        raise PngError("zero dimension")
    if depth != 8:
        raise PngError("bit depth != 8")
    if color != 3:
        raise PngError("color type != 3")
    if comp != 0:
        raise PngError("compression != 0")
    if filt != 0:
        raise PngError("filter != 0")
    if interlace != 0:
        raise PngError("interlace != 0")

    iend_count = sum(1 for t, _ in chunks if t == b"IEND")
    if iend_count != 1:
        raise PngError("IEND count != 1")
    if chunks[-1][0] != b"IEND" or len(chunks[-1][1]) != 0:
        raise PngError("bad IEND")

    plte = [c for t, c in chunks if t == b"PLTE"]
    if len(plte) != 1:
        raise PngError("PLTE count != 1")
    plte_data = plte[0]
    if len(plte_data) % 3 != 0:
        raise PngError("PLTE length not divisible by 3")
    if len(plte_data) > 768:
        raise PngError("PLTE too long")
    n_entries = len(plte_data) // 3

    idat_indices = [i for i, (t, _) in enumerate(chunks) if t == b"IDAT"]
    if not idat_indices:
        raise PngError("no IDAT")
    if idat_indices != list(range(idat_indices[0], idat_indices[-1] + 1)):
        raise PngError("IDATs not consecutive")

    plte_index = next(i for i, (t, _) in enumerate(chunks) if t == b"PLTE")
    if plte_index > idat_indices[0]:
        raise PngError("PLTE after IDAT")

    idat_data = b"".join(c for t, c in chunks if t == b"IDAT")
    try:
        raw = zlib.decompress(idat_data)
    except zlib.error as e:
        raise PngError("zlib decompress failed: %s" % e)

    expected = (width + 1) * height
    if len(raw) != expected:
        raise PngError("decompressed size %d != %d" % (len(raw), expected))

    stride = width + 1
    for y in range(height):
        row = raw[y * stride:(y + 1) * stride]
        ftype = row[0]
        if ftype > 4:
            raise PngError("bad filter byte %d in row %d" % (ftype, y))
        if ftype == 0:
            for x in range(width):
                if row[1 + x] >= n_entries:
                    raise PngError("palette index out of range in row %d" % y)

    return width, height


def main(argv):
    paths = argv[1:]
    if not paths:
        return 1
    failed = False
    for path in paths:
        try:
            w, h = validate(path)
        except PngError as e:
            print("FAIL %s: %s" % (path, e))
            failed = True
        else:
            print("ok %s %dx%d" % (path, w, h))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
