#include "savefile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wchar.h>
#else
#include <sys/stat.h>
#endif

#define NAME_CHARS 64

static int name_is_plain(const char *name)
{
    size_t i;
    size_t n = strlen(name);

    if (n == 0 || n >= NAME_CHARS || name[0] == '.')
        return 0;
    for (i = 0; i < n; i++) {
        char c = name[i];
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '.'))
            return 0;
    }
    return 1;
}

#ifdef _WIN32

#define DIR_CHARS 1024
static wchar_t g_dir[DIR_CHARS];

static int file_exists(const wchar_t *path)
{
    return GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
}

static void default_dir(void)
{
    wchar_t exe[DIR_CHARS];
    wchar_t marker[DIR_CHARS + 16];
    DWORD n = GetModuleFileNameW(NULL, exe, DIR_CHARS);
    const wchar_t *appdata;

    if (n > 0 && n < DIR_CHARS) {
        wchar_t *slash = wcsrchr(exe, L'\\');
        if (slash != NULL) {
            *slash = L'\0';
            swprintf(marker, DIR_CHARS + 16, L"%ls\\portable.txt", exe);
            if (file_exists(marker)) {
                swprintf(g_dir, DIR_CHARS, L"%ls", exe);
                return;
            }
        }
    }
    appdata = _wgetenv(L"APPDATA");
    swprintf(g_dir, DIR_CHARS, L"%ls\\Lofta", appdata != NULL ? appdata : L".");
}

void savefile_init(const char *dir)
{
    if (dir != NULL)
        MultiByteToWideChar(CP_UTF8, 0, dir, -1, g_dir, DIR_CHARS);
    else
        default_dir();
    CreateDirectoryW(g_dir, NULL);
}

static void make_path(wchar_t *out, size_t chars, const char *name, const wchar_t *suffix)
{
    wchar_t wide[NAME_CHARS];

    MultiByteToWideChar(CP_UTF8, 0, name, -1, wide, NAME_CHARS);
    swprintf(out, chars, L"%ls\\%ls%ls", g_dir, wide, suffix);
}

int savefile_write(const char *name, const char *data, size_t len)
{
    wchar_t path[DIR_CHARS + NAME_CHARS + 8];
    wchar_t tmp[DIR_CHARS + NAME_CHARS + 8];
    FILE *f;
    int ok;

    if (!name_is_plain(name))
        return 0;
    make_path(path, sizeof path / sizeof path[0], name, L"");
    make_path(tmp, sizeof tmp / sizeof tmp[0], name, L".tmp");

    f = _wfopen(tmp, L"wb");
    if (f == NULL)
        return 0;
    ok = fwrite(data, 1, len, f) == len;
    ok = fclose(f) == 0 && ok;
    if (!ok) {
        DeleteFileW(tmp);
        return 0;
    }
    return MoveFileExW(tmp, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
}

char *savefile_read(const char *name, size_t *len)
{
    wchar_t path[DIR_CHARS + NAME_CHARS + 8];
    FILE *f;
    long size;
    char *data;

    if (!name_is_plain(name))
        return NULL;
    make_path(path, sizeof path / sizeof path[0], name, L"");
    f = _wfopen(path, L"rb");
    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = size >= 0 ? (char *)malloc((size_t)size + 1) : NULL;
    if (data != NULL && fread(data, 1, (size_t)size, f) != (size_t)size) {
        free(data);
        data = NULL;
    }
    fclose(f);
    if (data != NULL)
        *len = (size_t)size;
    return data;
}

#else

#define DIR_CHARS 1024
static char g_dir[DIR_CHARS] = ".";

void savefile_init(const char *dir)
{
    snprintf(g_dir, sizeof g_dir, "%s", dir != NULL ? dir : ".");
    mkdir(g_dir, 0777);
}

int savefile_write(const char *name, const char *data, size_t len)
{
    char path[DIR_CHARS + NAME_CHARS + 8];
    char tmp[DIR_CHARS + NAME_CHARS + 8];
    FILE *f;
    int ok;

    if (!name_is_plain(name))
        return 0;
    snprintf(path, sizeof path, "%s/%s", g_dir, name);
    snprintf(tmp, sizeof tmp, "%s/%s.tmp", g_dir, name);

    f = fopen(tmp, "wb");
    if (f == NULL)
        return 0;
    ok = fwrite(data, 1, len, f) == len;
    ok = fclose(f) == 0 && ok;
    if (!ok) {
        remove(tmp);
        return 0;
    }
    return rename(tmp, path) == 0;
}

char *savefile_read(const char *name, size_t *len)
{
    char path[DIR_CHARS + NAME_CHARS + 8];
    FILE *f;
    long size;
    char *data;

    if (!name_is_plain(name))
        return NULL;
    snprintf(path, sizeof path, "%s/%s", g_dir, name);
    f = fopen(path, "rb");
    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = size >= 0 ? (char *)malloc((size_t)size + 1) : NULL;
    if (data != NULL && fread(data, 1, (size_t)size, f) != (size_t)size) {
        free(data);
        data = NULL;
    }
    fclose(f);
    if (data != NULL)
        *len = (size_t)size;
    return data;
}

#endif
