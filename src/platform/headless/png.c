#include "png.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PNG_MAX_DIM 16384

static unsigned long crc_table[256];
static int crc_table_ready = 0;

static void make_crc_table(void)
{
    unsigned long c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (unsigned long)n;
        for (k = 0; k < 8; k++) {
            if (c & 1UL) {
                c = 0xedb88320UL ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    crc_table_ready = 1;
}

static unsigned long crc_update(unsigned long crc, const unsigned char *buf, size_t len)
{
    size_t i;

    if (!crc_table_ready) {
        make_crc_table();
    }
    for (i = 0; i < len; i++) {
        crc = crc_table[(crc ^ buf[i]) & 0xffUL] ^ (crc >> 8);
    }
    return crc;
}

static unsigned long crc_calc(const unsigned char *buf, size_t len)
{
    return crc_update(0xffffffffUL, buf, len) ^ 0xffffffffUL;
}

static unsigned long adler32_calc(const unsigned char *buf, size_t len)
{
    unsigned long a = 1;
    unsigned long b = 0;
    size_t i;

    for (i = 0; i < len; i++) {
        a = (a + buf[i]) % 65521UL;
        b = (b + a) % 65521UL;
    }
    return (b << 16) | a;
}

static void put_u32(unsigned char *p, unsigned long v)
{
    p[0] = (unsigned char)((v >> 24) & 0xffUL);
    p[1] = (unsigned char)((v >> 16) & 0xffUL);
    p[2] = (unsigned char)((v >> 8) & 0xffUL);
    p[3] = (unsigned char)(v & 0xffUL);
}

static int write_chunk(FILE *f, const char type[4], const unsigned char *data, unsigned long len)
{
    unsigned char hdr[8];
    unsigned char tail[4];
    unsigned long crc;

    put_u32(hdr, len);
    memcpy(hdr + 4, type, 4);
    if (fwrite(hdr, 1, 8, f) != 8) {
        return 0;
    }
    if (len > 0 && fwrite(data, 1, (size_t)len, f) != (size_t)len) {
        return 0;
    }
    crc = crc_calc(hdr + 4, 4);
    if (len > 0) {
        crc = crc_update(crc ^ 0xffffffffUL, data, (size_t)len) ^ 0xffffffffUL;
        /* crc_calc over type only was already finalized; recompute properly below */
    }
    /* Recompute CRC over type + data in one pass to avoid finalization bugs. */
    {
        unsigned long c = 0xffffffffUL;
        c = crc_update(c, hdr + 4, 4);
        if (len > 0) {
            c = crc_update(c, data, (size_t)len);
        }
        crc = c ^ 0xffffffffUL;
    }
    put_u32(tail, crc);
    if (fwrite(tail, 1, 4, f) != 4) {
        return 0;
    }
    return 1;
}

int png_write_indexed(const char *path, const unsigned char *pixels, int w, int h,
                      unsigned char pal[256][3], int scale)
{
    static const unsigned char sig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    unsigned char ihdr[13];
    unsigned char plte[256 * 3];
    unsigned long row_bytes;
    unsigned long raw_size;
    unsigned long zlib_size;
    unsigned char *raw;
    unsigned char *z;
    unsigned char *row;
    unsigned long raw_pos;
    unsigned long zpos;
    unsigned long remaining;
    unsigned long adler;
    int ow, oh, x, y, sx, sy;
    FILE *f;

    if (path == NULL || pixels == NULL || pal == NULL) {
        return 0;
    }
    if (w < 1 || h < 1 || scale < 1) {
        return 0;
    }
    if (scale > PNG_MAX_DIM) {
        return 0;
    }
    ow = w * scale;
    oh = h * scale;
    if (ow > PNG_MAX_DIM || oh > PNG_MAX_DIM) {
        return 0;
    }

    row_bytes = (unsigned long)ow + 1UL;
    raw_size = row_bytes * (unsigned long)oh;
    {
        unsigned long blocks = (raw_size + 65534UL) / 65535UL;
        if (blocks == 0) {
            blocks = 1;
        }
        zlib_size = 2UL + 4UL + blocks * 5UL + raw_size + 4UL;
    }

    raw = (unsigned char *)malloc((size_t)raw_size);
    z = (unsigned char *)malloc((size_t)zlib_size);
    row = (unsigned char *)malloc((size_t)row_bytes);
    if (raw == NULL || z == NULL || row == NULL) {
        free(raw);
        free(z);
        free(row);
        return 0;
    }

    raw_pos = 0;
    for (y = 0; y < oh; y++) {
        sy = y / scale;
        raw[raw_pos++] = 0;
        for (x = 0; x < ow; x++) {
            sx = x / scale;
            raw[raw_pos++] = pixels[(unsigned long)sy * (unsigned long)w + (unsigned long)sx];
        }
    }

    adler = adler32_calc(raw, (size_t)raw_size);

    zpos = 0;
    z[zpos++] = 0x78;
    z[zpos++] = 0x01;
    remaining = raw_size;
    {
        unsigned long off = 0;
        do {
            unsigned long blk;
            unsigned char bfinal;

            if (remaining > 65535UL) {
                blk = 65535UL;
                bfinal = 0;
            } else {
                blk = remaining;
                bfinal = 1;
            }
            z[zpos++] = bfinal;
            z[zpos++] = (unsigned char)(blk & 0xffUL);
            z[zpos++] = (unsigned char)((blk >> 8) & 0xffUL);
            {
                unsigned short nlen = (unsigned short)(~(unsigned short)blk & 0xffffU);
                z[zpos++] = (unsigned char)(nlen & 0xffU);
                z[zpos++] = (unsigned char)((nlen >> 8) & 0xffU);
            }
            if (blk > 0) {
                memcpy(z + zpos, raw + off, (size_t)blk);
                zpos += blk;
            }
            off += blk;
            remaining -= blk;
        } while (remaining > 0);
    }
    z[zpos++] = (unsigned char)((adler >> 24) & 0xffUL);
    z[zpos++] = (unsigned char)((adler >> 16) & 0xffUL);
    z[zpos++] = (unsigned char)((adler >> 8) & 0xffUL);
    z[zpos++] = (unsigned char)(adler & 0xffUL);

    ihdr[0] = (unsigned char)((ow >> 24) & 0xff);
    ihdr[1] = (unsigned char)((ow >> 16) & 0xff);
    ihdr[2] = (unsigned char)((ow >> 8) & 0xff);
    ihdr[3] = (unsigned char)(ow & 0xff);
    ihdr[4] = (unsigned char)((oh >> 24) & 0xff);
    ihdr[5] = (unsigned char)((oh >> 16) & 0xff);
    ihdr[6] = (unsigned char)((oh >> 8) & 0xff);
    ihdr[7] = (unsigned char)(oh & 0xff);
    ihdr[8] = 8;
    ihdr[9] = 3;
    ihdr[10] = 0;
    ihdr[11] = 0;
    ihdr[12] = 0;

    {
        int i, j;
        for (i = 0; i < 256; i++) {
            for (j = 0; j < 3; j++) {
                plte[i * 3 + j] = pal[i][j];
            }
        }
    }

    f = fopen(path, "wb");
    if (f == NULL) {
        free(raw);
        free(z);
        free(row);
        return 0;
    }

    if (fwrite(sig, 1, 8, f) != 8) {
        goto fail;
    }
    if (!write_chunk(f, "IHDR", ihdr, 13UL)) {
        goto fail;
    }
    if (!write_chunk(f, "PLTE", plte, 256UL * 3UL)) {
        goto fail;
    }
    if (!write_chunk(f, "IDAT", z, zpos)) {
        goto fail;
    }
    if (!write_chunk(f, "IEND", NULL, 0UL)) {
        goto fail;
    }

    if (fclose(f) != 0) {
        free(raw);
        free(z);
        free(row);
        return 0;
    }
    free(raw);
    free(z);
    free(row);
    return 1;

fail:
    fclose(f);
    free(raw);
    free(z);
    free(row);
    return 0;
}
