/* Minimal PNG writer for test screenshots. */
#ifndef LOFTA_PNG_H
#define LOFTA_PNG_H

/* Writes an 8-bit indexed PNG with a 256-entry palette. pixels holds w * h bytes,
 * row-major. Each source pixel becomes a scale x scale block. Returns 1 on success
 * and 0 on bad arguments (NULL pointers, w, h or scale < 1, output larger than
 * 16384 pixels on a side) or a file error. */
int png_write_indexed(const char *path, const unsigned char *pixels, int w, int h,
                      unsigned char pal[256][3], int scale);

#endif
