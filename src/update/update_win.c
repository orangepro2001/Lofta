#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "relhub.h"
#include "update.h"
#include "version.h"

#define APP_ID "lofta"
#define PLATFORM "win-x64"
#define DEFAULT_URL "https://tony-ai-pc-1.tail9f7f1f.ts.net/release"
#define DEFAULT_CHANNEL "stable"
/* The service's signing key. Pinned here on purpose: a key fetched from the server would
 * protect against nothing. */
#define PINNED_KEYS "k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8="

#define URL_CHARS 1024
#define PATH_CHARS 1024
#define BODY_MAX (256u * 1024u)
#define FIRST_CHECK_MS (20u * 1000u)
#define CHECK_EVERY_MS (6u * 60u * 60u * 1000u)
#define CLEANUP_TRIES 40
#define CLEANUP_WAIT_MS 250u
#define IO_CHUNK 65536u
#define TIMEOUT_MS 15000

typedef struct {
    char url[URL_CHARS];
    char channel[REL_MAX_ID];
    RelKeys keys;
    long long installed;
    int disabled;
} Settings;

static Settings g_settings;
static HANDLE g_thread;
static HANDLE g_stop;
static volatile LONG g_state; /* one of the STATE_ values */
static volatile LONG g_install_wanted;
static char g_offered[REL_MAX_NAME];
static wchar_t g_ready_path[PATH_CHARS];

enum { STATE_OFF, STATE_IDLE, STATE_CHECKING, STATE_LATEST, STATE_DOWNLOADING, STATE_READY, STATE_FAILED };

static const char *const STATE_NAMES[] = {"off", "idle", "checking", "latest", "downloading", "ready", "failed"};

static void log_line(const char *text)
{
    FILE *f = fopen("lofta_update.log", "a");

    if (f != NULL) {
        fprintf(f, "%s\n", text);
        fclose(f);
    }
}

static int to_wide(const char *text, wchar_t *out, int cap)
{
    return MultiByteToWideChar(CP_UTF8, 0, text, -1, out, cap) > 0;
}

/* Fetches a URL into memory (file NULL) or onto the end of an open file. */
static int http_get(const char *url, HANDLE file, char **body, size_t *body_len, DWORD *status_out)
{
    wchar_t wide[URL_CHARS];
    wchar_t host[256];
    wchar_t path[URL_CHARS];
    URL_COMPONENTS parts;
    HINTERNET session = NULL, connection = NULL, request = NULL;
    DWORD status = 0, size = sizeof status;
    char *buffer = NULL;
    size_t used = 0;
    int ok = 0;

    if (!to_wide(url, wide, URL_CHARS))
        return 0;
    memset(&parts, 0, sizeof parts);
    parts.dwStructSize = sizeof parts;
    parts.lpszHostName = host;
    parts.dwHostNameLength = sizeof host / sizeof host[0];
    parts.lpszUrlPath = path;
    parts.dwUrlPathLength = sizeof path / sizeof path[0];
    if (!WinHttpCrackUrl(wide, 0, 0, &parts))
        return 0;

    session = WinHttpOpen(L"Lofta-Updater/1", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                          WINHTTP_NO_PROXY_BYPASS, 0);
    if (session == NULL)
        goto done;
    WinHttpSetTimeouts(session, TIMEOUT_MS, TIMEOUT_MS, TIMEOUT_MS, TIMEOUT_MS * 4);
    connection = WinHttpConnect(session, host, parts.nPort, 0);
    if (connection == NULL)
        goto done;
    request = WinHttpOpenRequest(connection, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                 parts.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (request == NULL)
        goto done;
    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request, NULL))
        goto done;
    if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                             WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX))
        goto done;
    if (status_out != NULL)
        *status_out = status;
    if (status != 200)
        goto done;

    if (file == NULL) {
        buffer = (char *)malloc(BODY_MAX + 1);
        if (buffer == NULL)
            goto done;
    }
    for (;;) {
        static char chunk[IO_CHUNK];
        DWORD got = 0;

        if (!WinHttpReadData(request, chunk, sizeof chunk, &got))
            goto done;
        if (got == 0)
            break;
        if (file != NULL) {
            DWORD written = 0;

            if (!WriteFile(file, chunk, got, &written, NULL) || written != got)
                goto done;
        } else {
            if (used + got > BODY_MAX)
                goto done;
            memcpy(buffer + used, chunk, got);
            used += got;
        }
    }
    if (file == NULL) {
        buffer[used] = '\0';
        *body = buffer;
        *body_len = used;
        buffer = NULL;
    }
    ok = 1;
done:
    free(buffer);
    if (request != NULL)
        WinHttpCloseHandle(request);
    if (connection != NULL)
        WinHttpCloseHandle(connection);
    if (session != NULL)
        WinHttpCloseHandle(session);
    return ok;
}

static void exe_path(wchar_t *out, DWORD cap)
{
    DWORD n = GetModuleFileNameW(NULL, out, cap);

    if (n == 0 || n >= cap)
        out[0] = L'\0';
}

static void exe_dir(wchar_t *out, DWORD cap)
{
    wchar_t *slash;

    exe_path(out, cap);
    slash = wcsrchr(out, L'\\');
    if (slash != NULL)
        *slash = L'\0';
}

/* Checks the signed answer; returns 1 and fills check when it is valid, whatever it offers. */
static int check_service(const Settings *s, RelCheck *check, char *why, size_t why_size)
{
    char url[URL_CHARS + 128];
    char *body = NULL;
    size_t length = 0;
    DWORD status = 0;
    RelResult r;

    snprintf(url, sizeof url, "%s/v1/apps/" APP_ID "/check?platform=" PLATFORM "&channel=%s&versionCode=%lld",
             s->url, s->channel, s->installed);
    if (!http_get(url, NULL, &body, &length, &status)) {
        snprintf(why, why_size, "check failed, http status %lu", (unsigned long)status);
        return 0;
    }
    r = rel_parse_check(body, length, check);
    free(body);
    if (r == REL_OK && !check->has_manifest)
        r = REL_ERR_JSON;
    if (r == REL_OK)
        r = rel_verify(&check->manifest, &s->keys, APP_ID, PLATFORM, s->channel);
    if (r == REL_OK && !rel_download_path_ok(check->download_path))
        r = REL_ERR_FIELDS;
    if (r != REL_OK) {
        snprintf(why, why_size, "invalid_argument: %s", rel_result_name(r));
        return 0;
    }
    return 1;
}

static int file_digest(HANDLE file, long long *size, char hex[65])
{
    static char chunk[IO_CHUNK];
    unsigned char digest[32];
    RelSha256 sha;
    LARGE_INTEGER zero;
    long long total = 0;

    zero.QuadPart = 0;
    if (!SetFilePointerEx(file, zero, NULL, FILE_BEGIN))
        return 0;
    rel_sha256_init(&sha);
    for (;;) {
        DWORD got = 0;

        if (!ReadFile(file, chunk, sizeof chunk, &got, NULL))
            return 0;
        if (got == 0)
            break;
        rel_sha256_update(&sha, chunk, got);
        total += got;
    }
    rel_sha256_final(&sha, digest);
    rel_sha256_hex(digest, hex);
    *size = total;
    return 1;
}

/* Downloads into updates\ beside the program and proves size and SHA-256 before anything may
 * use the file. A failed file is deleted. */
static int download_verified(const Settings *s, const RelCheck *check, wchar_t *ready, char *why, size_t why_size)
{
    wchar_t dir[PATH_CHARS];
    wchar_t name[REL_MAX_NAME];
    char url[URL_CHARS + REL_MAX_PATH];
    char hex[65];
    long long size = 0;
    HANDLE file;
    DWORD status = 0;
    int ok;

    exe_dir(dir, PATH_CHARS);
    wcsncat(dir, L"\\updates", PATH_CHARS - wcslen(dir) - 1);
    CreateDirectoryW(dir, NULL);
    if (!to_wide(check->manifest.file, name, REL_MAX_NAME))
        return 0;
    swprintf(ready, PATH_CHARS, L"%ls\\%ls", dir, name);
    file = CreateFileW(ready, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        snprintf(why, why_size, "cannot write the update beside the program");
        return 0;
    }
    snprintf(url, sizeof url, "%s/%s", s->url, check->download_path);
    ok = http_get(url, file, NULL, NULL, &status);
    if (ok)
        ok = file_digest(file, &size, hex);
    CloseHandle(file);
    if (!ok) {
        snprintf(why, why_size, "download failed, http status %lu", (unsigned long)status);
    } else if (size != check->manifest.size || strcmp(hex, check->manifest.sha256) != 0) {
        snprintf(why, why_size, "downloaded file does not match the signed size and hash");
        ok = 0;
    }
    if (!ok)
        DeleteFileW(ready);
    return ok;
}

/* A running program cannot be overwritten but can be renamed: move it aside, move the verified
 * file in, start it. On failure the old program goes back. */
static int install_and_restart(const wchar_t *ready, const char *old_name, const wchar_t *extra)
{
    wchar_t exe[PATH_CHARS], old[PATH_CHARS + 8], command[PATH_CHARS * 2];
    wchar_t from[REL_MAX_NAME];
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    exe_path(exe, PATH_CHARS);
    swprintf(old, PATH_CHARS + 8, L"%ls.old", exe);
    DeleteFileW(old);
    if (!MoveFileExW(exe, old, MOVEFILE_REPLACE_EXISTING))
        return 0;
    if (!MoveFileExW(ready, exe, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) {
        MoveFileExW(old, exe, MOVEFILE_REPLACE_EXISTING);
        return 0;
    }
    to_wide(old_name, from, REL_MAX_NAME);
    swprintf(command, PATH_CHARS * 2, L"\"%ls\" --updated-from=%ls %ls", exe, from, extra != NULL ? extra : L"");
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    if (!CreateProcessW(exe, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        DeleteFileW(exe);
        MoveFileExW(old, exe, MOVEFILE_REPLACE_EXISTING);
        return 0;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 1;
}

/* The old program may still be exiting, so its removal is retried. Returns 1 when it is gone. */
static int cleanup_after_update(void)
{
    wchar_t exe[PATH_CHARS], old[PATH_CHARS + 8], dir[PATH_CHARS];
    int i;

    exe_path(exe, PATH_CHARS);
    swprintf(old, PATH_CHARS + 8, L"%ls.old", exe);
    exe_dir(dir, PATH_CHARS);
    wcsncat(dir, L"\\updates", PATH_CHARS - wcslen(dir) - 1);
    for (i = 0; i < CLEANUP_TRIES; i++) {
        if (DeleteFileW(old) || GetLastError() == ERROR_FILE_NOT_FOUND)
            break;
        Sleep(CLEANUP_WAIT_MS);
    }
    RemoveDirectoryW(dir);
    return GetFileAttributesW(old) == INVALID_FILE_ATTRIBUTES;
}

static int settings_load(Settings *s, const char *url, const char *channel, const char *keys, long long installed)
{
    const char *value;

    memset(s, 0, sizeof *s);
    if (url == NULL)
        url = getenv("LOFTA_UPDATE_URL");
    if (channel == NULL)
        channel = getenv("LOFTA_UPDATE_CHANNEL");
    if (keys == NULL)
        keys = getenv("LOFTA_UPDATE_KEYS");
    value = url != NULL ? url : DEFAULT_URL;
    if (strcmp(value, "off") == 0)
        s->disabled = 1;
    snprintf(s->url, sizeof s->url, "%s", value);
    while (strlen(s->url) > 0 && s->url[strlen(s->url) - 1] == '/')
        s->url[strlen(s->url) - 1] = '\0';
    snprintf(s->channel, sizeof s->channel, "%s", channel != NULL ? channel : DEFAULT_CHANNEL);
    if (rel_keys_parse(&s->keys, keys != NULL ? keys : PINNED_KEYS) <= 0)
        return 0;
    s->installed = installed;
    return 1;
}

static int write_marker(const char *path, const char *text)
{
    FILE *f = fopen(path, "w");

    if (f == NULL)
        return 0;
    fputs(text, f);
    fclose(f);
    return 1;
}

static const char *flag_value(const char *arg, const char *name)
{
    size_t n = strlen(name);

    if (strncmp(arg, name, n) == 0 && arg[n] == '=')
        return arg + n + 1;
    return NULL;
}

/* --update=URL [--update-channel=C] [--update-keys=K] [--update-installed-code=N]
 * [--update-auto-install] [--update-test-marker=FILE]: one headless check, for tests. */
static int run_headless_update(const char *url, const char *channel, const char *keys, long long installed,
                               int auto_install, const char *marker)
{
    Settings s;
    static RelCheck check;
    char why[256] = "";
    char line[512];
    wchar_t ready[PATH_CHARS];
    wchar_t extra[PATH_CHARS];
    wchar_t wide_marker[PATH_CHARS];

    if (!settings_load(&s, url, channel, keys, installed)) {
        snprintf(line, sizeof line, "error bad keys\n");
        goto fail;
    }
    if (!check_service(&s, &check, why, sizeof why)) {
        snprintf(line, sizeof line, "error %s\n", why);
        goto fail;
    }
    if (!rel_update_available(&check.manifest, s.installed)) {
        snprintf(line, sizeof line, "latest %s %lld\n", check.manifest.version_name,
                 (long long)check.manifest.version_code);
        if (marker != NULL)
            write_marker(marker, line);
        return 0;
    }
    snprintf(line, sizeof line, "offered %s %lld mandatory=%d\n", check.manifest.version_name,
             (long long)check.manifest.version_code, rel_update_mandatory(&check.manifest, s.installed));
    if (!download_verified(&s, &check, ready, why, sizeof why)) {
        snprintf(line, sizeof line, "error %s\n", why);
        goto fail;
    }
    if (auto_install) {
        extra[0] = L'\0';
        if (marker != NULL && to_wide(marker, wide_marker, PATH_CHARS))
            swprintf(extra, PATH_CHARS, L"--print-version=\"%ls\"", wide_marker);
        if (!install_and_restart(ready, LOFTA_VERSION_NAME, extra)) {
            snprintf(line, sizeof line, "error install failed\n");
            goto fail;
        }
        return 0;
    }
    DeleteFileW(ready);
    if (marker != NULL)
        write_marker(marker, line);
    return 0;
fail:
    if (marker != NULL)
        write_marker(marker, line);
    return 1;
}

int update_run_command_line(int argc, char **argv)
{
    const char *url = NULL, *channel = NULL, *keys = NULL, *marker = NULL, *print_version = NULL;
    const char *value;
    long long installed = LOFTA_VERSION_CODE;
    int auto_install = 0, updated = 0;
    int i;

    for (i = 1; i < argc; i++) {
        if ((value = flag_value(argv[i], "--update")) != NULL)
            url = value;
        else if ((value = flag_value(argv[i], "--update-channel")) != NULL)
            channel = value;
        else if ((value = flag_value(argv[i], "--update-keys")) != NULL)
            keys = value;
        else if ((value = flag_value(argv[i], "--update-installed-code")) != NULL)
            installed = atoll(value);
        else if ((value = flag_value(argv[i], "--update-test-marker")) != NULL)
            marker = value;
        else if ((value = flag_value(argv[i], "--print-version")) != NULL)
            print_version = value;
        else if (flag_value(argv[i], "--updated-from") != NULL)
            updated = 1;
        else if (strcmp(argv[i], "--update-auto-install") == 0)
            auto_install = 1;
    }
    if (updated || print_version != NULL) {
        int removed = cleanup_after_update();

        if (print_version != NULL) {
            char line[128];

            snprintf(line, sizeof line, "%s %d old-removed=%s\n", LOFTA_VERSION_NAME, LOFTA_VERSION_CODE,
                     removed ? "true" : "false");
            write_marker(print_version, line);
            return 0;
        }
    }
    if (url != NULL)
        return run_headless_update(url, channel, keys, installed, auto_install, marker);
    return -1;
}

static DWORD WINAPI background(LPVOID unused)
{
    static RelCheck check;
    DWORD wait = FIRST_CHECK_MS;

    (void)unused;
    while (WaitForSingleObject(g_stop, wait) == WAIT_TIMEOUT) {
        char why[256] = "";

        wait = CHECK_EVERY_MS;
        InterlockedExchange(&g_state, STATE_CHECKING);
        if (!check_service(&g_settings, &check, why, sizeof why)) {
            /* Checking is best effort: note why and try again later, no dialogs. */
            log_line(why);
            InterlockedExchange(&g_state, STATE_FAILED);
            continue;
        }
        if (!rel_update_available(&check.manifest, g_settings.installed)) {
            InterlockedExchange(&g_state, STATE_LATEST);
            continue;
        }
        InterlockedExchange(&g_state, STATE_DOWNLOADING);
        if (!download_verified(&g_settings, &check, g_ready_path, why, sizeof why)) {
            log_line(why);
            InterlockedExchange(&g_state, STATE_FAILED);
            continue;
        }
        snprintf(g_offered, sizeof g_offered, "%s", check.manifest.version_name);
        InterlockedExchange(&g_state, STATE_READY);
        break;
    }
    return 0;
}

void update_start_background(void)
{
    if (LOFTA_VERSION_CODE == 0)
        return;
    if (!settings_load(&g_settings, NULL, NULL, NULL, LOFTA_VERSION_CODE) || g_settings.disabled)
        return;
    g_stop = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (g_stop == NULL)
        return;
    InterlockedExchange(&g_state, STATE_IDLE);
    g_thread = CreateThread(NULL, 0, background, NULL, 0, NULL);
}

const char *update_status(char *offered, size_t size)
{
    LONG state = g_state;

    if (state == STATE_READY)
        snprintf(offered, size, "%s", g_offered);
    return STATE_NAMES[state];
}

void update_request_install(void)
{
    if (g_state == STATE_READY)
        InterlockedExchange(&g_install_wanted, 1);
}

void update_shutdown(void)
{
    if (g_thread == NULL)
        return;
    SetEvent(g_stop);
    WaitForSingleObject(g_thread, 5000);
    CloseHandle(g_thread);
    g_thread = NULL;
}

int update_finish_install(void)
{
    if (g_install_wanted && !install_and_restart(g_ready_path, LOFTA_VERSION_NAME, NULL))
        log_line("install failed");
    return 0;
}
