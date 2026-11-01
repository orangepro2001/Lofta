/* Files in the save folder. A write goes to a temporary file first and replaces the old one
 * in a single step, so a crash leaves the old save or the new one, never half of either. */
#ifndef LOFTA_SAVEFILE_H
#define LOFTA_SAVEFILE_H

#include <stddef.h>

/* dir is the folder to use, or NULL for the default: next to the program when portable.txt
 * is there, otherwise %APPDATA%\Lofta. Creates the folder. */
void savefile_init(const char *dir);

/* name must be a plain file name such as "save.dat". Returns 1 on success. */
int savefile_write(const char *name, const char *data, size_t len);

/* Returns a malloc'd buffer the caller frees, or NULL when there is no such file. */
char *savefile_read(const char *name, size_t *len);

#endif
