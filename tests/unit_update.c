/* The update protocol against a real answer from a throwaway relhub (tests/fixtures), and
 * against every way the answer can be wrong. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "relhub.h"

static int g_failed;
static int g_checks;

#define CHECK(cond)                                                     \
    do {                                                                \
        g_checks++;                                                     \
        if (!(cond)) {                                                  \
            g_failed++;                                                 \
            fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #cond);  \
        }                                                               \
    } while (0)

static char *read_all(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    char *data;
    long n;

    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)malloc((size_t)n + 1);
    if (data != NULL && fread(data, 1, (size_t)n, f) != (size_t)n) {
        free(data);
        data = NULL;
    }
    fclose(f);
    if (data != NULL) {
        data[n] = '\0';
        *size = (size_t)n;
    }
    return data;
}

static void hex_of(const char *text, char out[65])
{
    unsigned char digest[32];
    RelSha256 s;

    rel_sha256_init(&s);
    rel_sha256_update(&s, text, strlen(text));
    rel_sha256_final(&s, digest);
    rel_sha256_hex(digest, out);
}

static void test_sha256(void)
{
    char hex[65];
    static char million[1000001];

    hex_of("", hex);
    CHECK(strcmp(hex, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") == 0);
    hex_of("abc", hex);
    CHECK(strcmp(hex, "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad") == 0);
    hex_of("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", hex);
    CHECK(strcmp(hex, "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1") == 0);
    memset(million, 'a', 1000000);
    hex_of(million, hex);
    CHECK(strcmp(hex, "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0") == 0);
}

static void test_base64(void)
{
    unsigned char out[8];

    CHECK(rel_base64_decode("TWE=", out, 8) == 2 && memcmp(out, "Ma", 2) == 0);
    CHECK(rel_base64_decode("TQ==", out, 8) == 1 && out[0] == 'M');
    CHECK(rel_base64_decode("TWFu", out, 8) == 3);
    CHECK(rel_base64_decode("TWF", out, 8) == -1);
    CHECK(rel_base64_decode("T=Fu", out, 8) == -1);
    CHECK(rel_base64_decode("TWFu", out, 2) == -1);
    CHECK(rel_base64_decode("", out, 8) == -1);
}

static void test_answer(const char *dir)
{
    char path[512];
    char *json, *key_text;
    size_t size = 0, key_size = 0;
    static RelCheck check, copy;
    RelKeys keys, wrong;

    snprintf(path, sizeof path, "%s/check.json", dir);
    json = read_all(path, &size);
    snprintf(path, sizeof path, "%s/key.txt", dir);
    key_text = read_all(path, &key_size);
    CHECK(json != NULL && key_text != NULL);
    if (json == NULL || key_text == NULL)
        return;
    while (key_size > 0 && (key_text[key_size - 1] == '\n' || key_text[key_size - 1] == '\r'))
        key_text[--key_size] = '\0';

    memset(&keys, 0, sizeof keys);
    CHECK(rel_keys_parse(&keys, key_text) == 1);
    memset(&wrong, 0, sizeof wrong);
    CHECK(rel_keys_parse(&wrong, "k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8=") == 1);

    CHECK(rel_parse_check(json, size, &check) == REL_OK);
    CHECK(check.has_manifest);
    CHECK(strcmp(check.manifest.app, "lofta") == 0);
    CHECK(strcmp(check.manifest.platform, "win-x64") == 0);
    CHECK(rel_download_path_ok(check.download_path));

    /* The genuine answer verifies, and only with the right key and target. */
    CHECK(rel_verify(&check.manifest, &keys, "lofta", "win-x64", "beta") == REL_OK);
    CHECK(rel_verify(&check.manifest, &wrong, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    CHECK(rel_verify(&check.manifest, &keys, "other", "win-x64", "beta") == REL_ERR_TARGET);
    CHECK(rel_verify(&check.manifest, &keys, "lofta", "android", "beta") == REL_ERR_TARGET);
    CHECK(rel_verify(&check.manifest, &keys, "lofta", "win-x64", "stable") == REL_ERR_TARGET);

    /* Every signed field is really signed. */
    copy = check;
    copy.manifest.version_code += 1;
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    copy.manifest.min_version_code = 0 + 1;
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    copy.manifest.size += 1;
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    copy.manifest.sha256[0] = copy.manifest.sha256[0] == 'a' ? 'b' : 'a';
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    strcat(copy.manifest.notes, "x");
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    strcat(copy.manifest.file, "x");
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);
    copy = check;
    copy.manifest.signature[0] = '\0';
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_SIGNATURE);

    /* Field rules come before the signature. */
    copy = check;
    strcpy(copy.manifest.file, "../evil.exe");
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_FIELDS);
    copy = check;
    strcpy(copy.manifest.published_at, "2026-11-01T00:00:00Z\nsize=1");
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_FIELDS);
    copy = check;
    copy.manifest.min_version_code = copy.manifest.version_code + 1;
    CHECK(rel_verify(&copy.manifest, &keys, "lofta", "win-x64", "beta") == REL_ERR_FIELDS);

    /* The verdict. */
    CHECK(rel_update_available(&check.manifest, check.manifest.version_code - 1));
    CHECK(!rel_update_available(&check.manifest, check.manifest.version_code));
    CHECK(!rel_update_mandatory(&check.manifest, 0));

    /* A truncated or garbled answer is refused, at every length. */
    {
        size_t n, whole = size;

        /* Trailing whitespace is not part of the document. */
        while (whole > 0 && (json[whole - 1] == '\n' || json[whole - 1] == ' '))
            whole--;
        for (n = 0; n < whole; n++) {
            static RelCheck cut;
            RelResult r = rel_parse_check(json, n, &cut);

            CHECK(r == REL_ERR_JSON);
        }
    }
    free(json);
    free(key_text);
}

static void test_paths(void)
{
    CHECK(rel_download_path_ok("v1/apps/lofta/win-x64/releases/100/download"));
    CHECK(!rel_download_path_ok("https://evil/x"));
    CHECK(!rel_download_path_ok("v1/apps/../../secret"));
    CHECK(!rel_download_path_ok("v2/apps/a"));
    CHECK(!rel_download_path_ok("v1/apps/a b"));
}

static void test_keys(void)
{
    RelKeys keys;

    memset(&keys, 0, sizeof keys);
    CHECK(rel_keys_parse(&keys, "k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8=,k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8=") == 2);
    memset(&keys, 0, sizeof keys);
    CHECK(rel_keys_parse(&keys, "not-base64!") == -1);
    CHECK(rel_keys_parse(&keys, "TWFu") == -1);
}

int main(int argc, char **argv)
{
    test_sha256();
    test_base64();
    test_paths();
    test_keys();
    if (argc > 1)
        test_answer(argv[1]);
    printf("unit_update: %d checks, %d failed\n", g_checks, g_failed);
    return g_failed != 0;
}
