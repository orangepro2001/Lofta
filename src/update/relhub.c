#include "relhub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "monocypher-ed25519.h"

#define MAX_SIZE_BYTES ((int64_t)16 * 1024 * 1024 * 1024)
#define SIGNED_PAYLOAD_CAP 2048
#define SIGNATURE_BYTES 64

/* A small JSON reader: enough to walk objects, skip what we do not need and decode strings. */
typedef struct {
    const char *p;
    const char *end;
} Reader;

static void skip_space(Reader *r)
{
    while (r->p < r->end && (*r->p == ' ' || *r->p == '\n' || *r->p == '\r' || *r->p == '\t'))
        r->p++;
}

static int peek(Reader *r)
{
    skip_space(r);
    return r->p < r->end ? (unsigned char)*r->p : -1;
}

static int eat(Reader *r, char c)
{
    if (peek(r) != (unsigned char)c)
        return 0;
    r->p++;
    return 1;
}

static int hex_value(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

static int read_hex4(Reader *r, unsigned int *out)
{
    unsigned int v = 0;
    int i;

    if (r->end - r->p < 4)
        return 0;
    for (i = 0; i < 4; i++) {
        int h = hex_value((unsigned char)r->p[i]);

        if (h < 0)
            return 0;
        v = v * 16u + (unsigned int)h;
    }
    r->p += 4;
    *out = v;
    return 1;
}

static int put_byte(char *out, size_t cap, size_t *n, unsigned int c)
{
    if (out != NULL) {
        if (*n + 1 >= cap)
            return 0;
        out[*n] = (char)c;
    }
    (*n)++;
    return 1;
}

static int put_utf8(char *out, size_t cap, size_t *n, unsigned int cp)
{
    if (cp < 0x80u)
        return put_byte(out, cap, n, cp);
    if (cp < 0x800u)
        return put_byte(out, cap, n, 0xC0u | (cp >> 6)) && put_byte(out, cap, n, 0x80u | (cp & 0x3Fu));
    if (cp < 0x10000u)
        return put_byte(out, cap, n, 0xE0u | (cp >> 12)) &&
               put_byte(out, cap, n, 0x80u | ((cp >> 6) & 0x3Fu)) &&
               put_byte(out, cap, n, 0x80u | (cp & 0x3Fu));
    return put_byte(out, cap, n, 0xF0u | (cp >> 18)) &&
           put_byte(out, cap, n, 0x80u | ((cp >> 12) & 0x3Fu)) &&
           put_byte(out, cap, n, 0x80u | ((cp >> 6) & 0x3Fu)) &&
           put_byte(out, cap, n, 0x80u | (cp & 0x3Fu));
}

/* Reads a string into out (NULL to skip it). Refuses a string that does not fit: a truncated
 * field would change the signed bytes without changing what we show. */
static int read_string(Reader *r, char *out, size_t cap)
{
    size_t n = 0;

    if (!eat(r, '"'))
        return 0;
    while (r->p < r->end) {
        unsigned char c = (unsigned char)*r->p++;

        if (c == '"') {
            if (out != NULL)
                out[n] = '\0';
            return 1;
        }
        if (c < 0x20)
            return 0;
        if (c != '\\') {
            if (!put_byte(out, cap, &n, c))
                return 0;
            continue;
        }
        if (r->p >= r->end)
            return 0;
        c = (unsigned char)*r->p++;
        switch (c) {
        case '"': case '\\': case '/':
            if (!put_byte(out, cap, &n, c))
                return 0;
            break;
        case 'b': if (!put_byte(out, cap, &n, 8)) return 0; break;
        case 'f': if (!put_byte(out, cap, &n, 12)) return 0; break;
        case 'n': if (!put_byte(out, cap, &n, 10)) return 0; break;
        case 'r': if (!put_byte(out, cap, &n, 13)) return 0; break;
        case 't': if (!put_byte(out, cap, &n, 9)) return 0; break;
        case 'u': {
            unsigned int cp;

            if (!read_hex4(r, &cp))
                return 0;
            if (cp >= 0xD800u && cp < 0xDC00u) {
                unsigned int low;

                if (r->end - r->p < 2 || r->p[0] != '\\' || r->p[1] != 'u')
                    return 0;
                r->p += 2;
                if (!read_hex4(r, &low) || low < 0xDC00u || low > 0xDFFFu)
                    return 0;
                cp = 0x10000u + ((cp - 0xD800u) << 10) + (low - 0xDC00u);
            } else if (cp >= 0xDC00u && cp < 0xE000u) {
                return 0;
            }
            if (!put_utf8(out, cap, &n, cp))
                return 0;
            break;
        }
        default:
            return 0;
        }
    }
    return 0;
}

static int read_int(Reader *r, int64_t *out)
{
    int negative = 0;
    int64_t v = 0;
    int digits = 0;

    if (peek(r) == '-') {
        negative = 1;
        r->p++;
    }
    while (r->p < r->end && *r->p >= '0' && *r->p <= '9') {
        if (v > (INT64_MAX - 9) / 10)
            return 0;
        v = v * 10 + (*r->p - '0');
        r->p++;
        digits++;
    }
    if (digits == 0)
        return 0;
    *out = negative ? -v : v;
    return 1;
}

#define SKIP_DEPTH 16

static int skip_value(Reader *r, int depth)
{
    int c = peek(r);

    if (depth > SKIP_DEPTH)
        return 0;
    if (c == '"')
        return read_string(r, NULL, 0);
    if (c == '{' || c == '[') {
        int close = c == '{' ? '}' : ']';

        r->p++;
        if (peek(r) == close) {
            r->p++;
            return 1;
        }
        for (;;) {
            if (c == '{') {
                if (!read_string(r, NULL, 0) || !eat(r, ':'))
                    return 0;
            }
            if (!skip_value(r, depth + 1))
                return 0;
            if (peek(r) == ',') {
                r->p++;
                continue;
            }
            return eat(r, (char)close);
        }
    }
    /* Numbers and literals: anything up to a delimiter. */
    if (c < 0)
        return 0;
    while (r->p < r->end && strchr(",}] \n\r\t", *r->p) == NULL)
        r->p++;
    return 1;
}

static int read_manifest(Reader *r, RelManifest *m)
{
    if (!eat(r, '{'))
        return 0;
    if (peek(r) == '}') {
        r->p++;
        return 1;
    }
    for (;;) {
        char key[32];
        int ok = 1;

        if (!read_string(r, key, sizeof key) || !eat(r, ':'))
            return 0;
        if (strcmp(key, "app") == 0)
            ok = read_string(r, m->app, sizeof m->app);
        else if (strcmp(key, "platform") == 0)
            ok = read_string(r, m->platform, sizeof m->platform);
        else if (strcmp(key, "channel") == 0)
            ok = read_string(r, m->channel, sizeof m->channel);
        else if (strcmp(key, "versionCode") == 0)
            ok = read_int(r, &m->version_code);
        else if (strcmp(key, "versionName") == 0)
            ok = read_string(r, m->version_name, sizeof m->version_name);
        else if (strcmp(key, "file") == 0)
            ok = read_string(r, m->file, sizeof m->file);
        else if (strcmp(key, "size") == 0)
            ok = read_int(r, &m->size);
        else if (strcmp(key, "sha256") == 0)
            ok = read_string(r, m->sha256, sizeof m->sha256);
        else if (strcmp(key, "minVersionCode") == 0)
            ok = read_int(r, &m->min_version_code);
        else if (strcmp(key, "publishedAt") == 0)
            ok = read_string(r, m->published_at, sizeof m->published_at);
        else if (strcmp(key, "notes") == 0)
            ok = read_string(r, m->notes, sizeof m->notes);
        else if (strcmp(key, "signature") == 0)
            ok = read_string(r, m->signature, sizeof m->signature);
        else
            ok = skip_value(r, 0);
        if (!ok)
            return 0;
        if (peek(r) == ',') {
            r->p++;
            continue;
        }
        return eat(r, '}');
    }
}

RelResult rel_parse_check(const char *json, size_t size, RelCheck *out)
{
    Reader r;

    memset(out, 0, sizeof *out);
    r.p = json;
    r.end = json + size;
    if (!eat(&r, '{'))
        return REL_ERR_JSON;
    if (peek(&r) == '}')
        return REL_OK;
    for (;;) {
        char key[32];
        int ok;

        if (!read_string(&r, key, sizeof key) || !eat(&r, ':'))
            return REL_ERR_JSON;
        if (strcmp(key, "manifest") == 0) {
            ok = read_manifest(&r, &out->manifest);
            out->has_manifest = ok;
        } else if (strcmp(key, "downloadPath") == 0) {
            ok = read_string(&r, out->download_path, sizeof out->download_path);
        } else {
            ok = skip_value(&r, 0);
        }
        if (!ok)
            return REL_ERR_JSON;
        if (peek(&r) == ',') {
            r.p++;
            continue;
        }
        return eat(&r, '}') ? REL_OK : REL_ERR_JSON;
    }
}

static int char_in_id(int c, int extra)
{
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == extra || c == '-';
}

static int id_ok(const char *s, int first_digit_ok, int extra, size_t max)
{
    size_t n = strlen(s);
    size_t i;

    if (n < 1 || n > max)
        return 0;
    if (!((s[0] >= 'a' && s[0] <= 'z') || (first_digit_ok && s[0] >= '0' && s[0] <= '9')))
        return 0;
    for (i = 1; i < n; i++)
        if (!char_in_id((unsigned char)s[i], extra))
            return 0;
    return 1;
}

static int file_ok(const char *s)
{
    size_t n = strlen(s);
    size_t i;

    if (n < 1 || n > 128 || strstr(s, "..") != NULL)
        return 0;
    for (i = 0; i < n; i++) {
        int c = (unsigned char)s[i];
        int alnum = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');

        if (!alnum && (i == 0 || (c != '.' && c != '_' && c != '-')))
            return 0;
    }
    return 1;
}

static int version_name_ok(const char *s)
{
    size_t n = strlen(s);
    size_t i;

    if (n < 1 || n > 64)
        return 0;
    for (i = 0; i < n; i++)
        if ((unsigned char)s[i] < 0x20 || s[i] == 0x7F)
            return 0;
    return 1;
}

static int sha_ok(const char *s)
{
    size_t i;

    if (strlen(s) != 64)
        return 0;
    for (i = 0; i < 64; i++)
        if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f')))
            return 0;
    return 1;
}

/* RFC 3339 shape: enough to refuse newlines and other things that could forge payload lines. */
static int time_ok(const char *s)
{
    size_t n = strlen(s);
    size_t i;

    if (n < 20)
        return 0;
    for (i = 0; i < n; i++) {
        char c = s[i];

        if (!((c >= '0' && c <= '9') || c == '-' || c == ':' || c == '.' || c == 'T' || c == 'Z' || c == '+'))
            return 0;
    }
    return s[4] == '-' && s[7] == '-' && s[10] == 'T';
}

static int fields_ok(const RelManifest *m)
{
    return id_ok(m->app, 0, '-', 32) && id_ok(m->platform, 1, '_', 24) && id_ok(m->channel, 0, '-', 16) &&
           file_ok(m->file) && version_name_ok(m->version_name) && m->size >= 1 &&
           m->size <= MAX_SIZE_BYTES && m->version_code >= 0 && m->min_version_code >= 0 &&
           m->min_version_code <= m->version_code && sha_ok(m->sha256) && time_ok(m->published_at);
}

size_t rel_signed_payload(const RelManifest *m, char *buf, size_t cap)
{
    unsigned char digest[32];
    char notes_hex[65];
    RelSha256 s;
    int n;

    rel_sha256_init(&s);
    rel_sha256_update(&s, m->notes, strlen(m->notes));
    rel_sha256_final(&s, digest);
    rel_sha256_hex(digest, notes_hex);
    n = snprintf(buf, cap,
                 "relhub-release-v1\n"
                 "app=%s\n"
                 "platform=%s\n"
                 "channel=%s\n"
                 "versionCode=%lld\n"
                 "versionName=%s\n"
                 "file=%s\n"
                 "size=%lld\n"
                 "sha256=%s\n"
                 "minVersionCode=%lld\n"
                 "publishedAt=%s\n"
                 "notesSha256=%s\n",
                 m->app, m->platform, m->channel, (long long)m->version_code, m->version_name, m->file,
                 (long long)m->size, m->sha256, (long long)m->min_version_code, m->published_at, notes_hex);
    if (n < 0 || (size_t)n >= cap)
        return 0;
    return (size_t)n;
}

RelResult rel_verify(const RelManifest *m, const RelKeys *keys, const char *app,
                     const char *platform, const char *channel)
{
    static char payload[SIGNED_PAYLOAD_CAP];
    unsigned char signature[SIGNATURE_BYTES];
    size_t length;
    int i;

    if (!fields_ok(m))
        return REL_ERR_FIELDS;
    if (strcmp(m->app, app) != 0 || strcmp(m->platform, platform) != 0 || strcmp(m->channel, channel) != 0)
        return REL_ERR_TARGET;
    if (rel_base64_decode(m->signature, signature, sizeof signature) != SIGNATURE_BYTES)
        return REL_ERR_SIGNATURE;
    length = rel_signed_payload(m, payload, sizeof payload);
    if (length == 0)
        return REL_ERR_FIELDS;
    for (i = 0; i < keys->count; i++)
        if (crypto_ed25519_check(signature, keys->keys[i], (const uint8_t *)payload, length) == 0)
            return REL_OK;
    return REL_ERR_SIGNATURE;
}

int rel_update_available(const RelManifest *m, int64_t installed_code)
{
    return m->version_code > installed_code;
}

int rel_update_mandatory(const RelManifest *m, int64_t installed_code)
{
    return m->min_version_code > installed_code;
}

int rel_download_path_ok(const char *path)
{
    size_t i;

    if (strncmp(path, "v1/apps/", 8) != 0 || strstr(path, "..") != NULL)
        return 0;
    for (i = 0; path[i] != '\0'; i++) {
        unsigned char c = (unsigned char)path[i];
        int ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '/' ||
                 c == '-' || c == '_' || c == '.';

        if (!ok)
            return 0;
    }
    return 1;
}

const char *rel_result_name(RelResult r)
{
    switch (r) {
    case REL_OK: return "ok";
    case REL_ERR_JSON: return "bad answer";
    case REL_ERR_FIELDS: return "bad manifest fields";
    case REL_ERR_TARGET: return "manifest for another target";
    case REL_ERR_SIGNATURE: return "signature is not from a trusted release key";
    default: return "unknown";
    }
}

int rel_keys_parse(RelKeys *keys, const char *text)
{
    int added = 0;

    while (*text != '\0') {
        char one[64];
        size_t n = strcspn(text, ",");

        if (n == 0 || n >= sizeof one || keys->count >= REL_MAX_KEYS)
            return -1;
        memcpy(one, text, n);
        one[n] = '\0';
        if (rel_base64_decode(one, keys->keys[keys->count], REL_KEY_BYTES) != REL_KEY_BYTES)
            return -1;
        keys->count++;
        added++;
        text += n;
        if (*text == ',')
            text++;
    }
    return added;
}

static int b64_value(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return -1;
}

int rel_base64_decode(const char *text, unsigned char *out, int cap)
{
    size_t n = strlen(text);
    size_t i;
    int written = 0;

    if (n == 0 || n % 4 != 0)
        return -1;
    for (i = 0; i < n; i += 4) {
        int a = b64_value((unsigned char)text[i]);
        int b = b64_value((unsigned char)text[i + 1]);
        int pad2 = text[i + 2] == '=';
        int pad3 = text[i + 3] == '=';
        int c = pad2 ? 0 : b64_value((unsigned char)text[i + 2]);
        int d = pad3 ? 0 : b64_value((unsigned char)text[i + 3]);
        unsigned int v;

        if (a < 0 || b < 0 || c < 0 || d < 0)
            return -1;
        if ((pad2 || pad3) && i + 4 != n)
            return -1;
        if (pad2 && !pad3)
            return -1;
        v = ((unsigned int)a << 18) | ((unsigned int)b << 12) | ((unsigned int)c << 6) | (unsigned int)d;
        if (written >= cap)
            return -1;
        out[written++] = (unsigned char)(v >> 16);
        if (!pad2) {
            if (written >= cap)
                return -1;
            out[written++] = (unsigned char)(v >> 8);
        }
        if (!pad3) {
            if (written >= cap)
                return -1;
            out[written++] = (unsigned char)v;
        }
    }
    return written;
}

static const uint32_t K[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u,
};

static uint32_t rotr(uint32_t x, int n)
{
    return (x >> n) | (x << (32 - n));
}

static void sha_block(uint32_t state[8], const unsigned char *p)
{
    uint32_t w[64];
    uint32_t v[8];
    int i;

    for (i = 0; i < 16; i++)
        w[i] = ((uint32_t)p[i * 4] << 24) | ((uint32_t)p[i * 4 + 1] << 16) | ((uint32_t)p[i * 4 + 2] << 8) |
               (uint32_t)p[i * 4 + 3];
    for (i = 16; i < 64; i++) {
        uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);

        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    memcpy(v, state, sizeof v);
    for (i = 0; i < 64; i++) {
        uint32_t s1 = rotr(v[4], 6) ^ rotr(v[4], 11) ^ rotr(v[4], 25);
        uint32_t ch = (v[4] & v[5]) ^ (~v[4] & v[6]);
        uint32_t t1 = v[7] + s1 + ch + K[i] + w[i];
        uint32_t s0 = rotr(v[0], 2) ^ rotr(v[0], 13) ^ rotr(v[0], 22);
        uint32_t maj = (v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]);
        uint32_t t2 = s0 + maj;

        v[7] = v[6];
        v[6] = v[5];
        v[5] = v[4];
        v[4] = v[3] + t1;
        v[3] = v[2];
        v[2] = v[1];
        v[1] = v[0];
        v[0] = t1 + t2;
    }
    for (i = 0; i < 8; i++)
        state[i] += v[i];
}

void rel_sha256_init(RelSha256 *s)
{
    static const uint32_t INIT[8] = {0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
                                     0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u};

    memcpy(s->state, INIT, sizeof INIT);
    s->bytes = 0;
    s->used = 0;
}

void rel_sha256_update(RelSha256 *s, const void *data, size_t size)
{
    const unsigned char *p = (const unsigned char *)data;

    s->bytes += size;
    while (size > 0) {
        size_t take = sizeof s->block - s->used;

        if (take > size)
            take = size;
        memcpy(s->block + s->used, p, take);
        s->used += take;
        p += take;
        size -= take;
        if (s->used == sizeof s->block) {
            sha_block(s->state, s->block);
            s->used = 0;
        }
    }
}

void rel_sha256_final(RelSha256 *s, unsigned char digest[32])
{
    uint64_t bits = s->bytes * 8u;
    int i;

    s->block[s->used++] = 0x80;
    if (s->used > 56) {
        memset(s->block + s->used, 0, sizeof s->block - s->used);
        sha_block(s->state, s->block);
        s->used = 0;
    }
    memset(s->block + s->used, 0, 56 - s->used);
    for (i = 0; i < 8; i++)
        s->block[56 + i] = (unsigned char)(bits >> (56 - i * 8));
    sha_block(s->state, s->block);
    for (i = 0; i < 8; i++) {
        digest[i * 4] = (unsigned char)(s->state[i] >> 24);
        digest[i * 4 + 1] = (unsigned char)(s->state[i] >> 16);
        digest[i * 4 + 2] = (unsigned char)(s->state[i] >> 8);
        digest[i * 4 + 3] = (unsigned char)s->state[i];
    }
}

void rel_sha256_hex(const unsigned char digest[32], char hex[65])
{
    static const char DIGITS[] = "0123456789abcdef";
    int i;

    for (i = 0; i < 32; i++) {
        hex[i * 2] = DIGITS[digest[i] >> 4];
        hex[i * 2 + 1] = DIGITS[digest[i] & 15];
    }
    hex[64] = '\0';
}
