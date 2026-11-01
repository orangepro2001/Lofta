/* The release-publish protocol without any network: parsing the check answer, checking the
 * signed manifest against pinned keys, deciding what to do. Portable, so the tests run it
 * under the sanitizers; docs/RELEASE.md and the service's API.md describe the format. */
#ifndef LOFTA_RELHUB_H
#define LOFTA_RELHUB_H

#include <stddef.h>
#include <stdint.h>

#define REL_MAX_ID 33
#define REL_MAX_NAME 129
#define REL_MAX_TIME 48
#define REL_MAX_PATH 256
#define REL_MAX_NOTES 8200
#define REL_KEY_BYTES 32
#define REL_MAX_KEYS 4

typedef struct {
    char app[REL_MAX_ID];
    char platform[REL_MAX_ID];
    char channel[REL_MAX_ID];
    int64_t version_code;
    char version_name[REL_MAX_NAME];
    char file[REL_MAX_NAME];
    int64_t size;
    char sha256[65];
    int64_t min_version_code;
    char published_at[REL_MAX_TIME];
    char notes[REL_MAX_NOTES];
    char signature[REL_MAX_NAME];
} RelManifest;

typedef struct {
    RelManifest manifest;
    int has_manifest;
    char download_path[REL_MAX_PATH];
} RelCheck;

typedef struct {
    unsigned char keys[REL_MAX_KEYS][REL_KEY_BYTES];
    int count;
} RelKeys;

typedef enum {
    REL_OK = 0,
    REL_ERR_JSON,      /* not the answer we know */
    REL_ERR_FIELDS,    /* breaks the field rules of the protocol */
    REL_ERR_TARGET,    /* signed, but for another app, platform or channel */
    REL_ERR_SIGNATURE  /* no pinned key vouches for it */
} RelResult;

/* Parses the body of GET v1/apps/{app}/check. */
RelResult rel_parse_check(const char *json, size_t size, RelCheck *out);

/* Adds comma-separated base64 public keys. Returns the number added, -1 on a bad key. */
int rel_keys_parse(RelKeys *keys, const char *text);

/* The bytes the signature covers; returns their length, 0 when buf is too small. */
size_t rel_signed_payload(const RelManifest *m, char *buf, size_t cap);

/* Field rules, target match and signature, in that order. */
RelResult rel_verify(const RelManifest *m, const RelKeys *keys, const char *app,
                     const char *platform, const char *channel);

/* The verdict comes from the signed fields only; the booleans in the answer are not signed. */
int rel_update_available(const RelManifest *m, int64_t installed_code);
int rel_update_mandatory(const RelManifest *m, int64_t installed_code);

/* The download path must stay inside the API, whatever the server said. */
int rel_download_path_ok(const char *path);

const char *rel_result_name(RelResult r);

/* Base64, standard alphabet with padding. Returns bytes written, -1 on bad input. */
int rel_base64_decode(const char *text, unsigned char *out, int cap);

/* SHA-256. */
typedef struct {
    uint32_t state[8];
    uint64_t bytes;
    unsigned char block[64];
    size_t used;
} RelSha256;

void rel_sha256_init(RelSha256 *s);
void rel_sha256_update(RelSha256 *s, const void *data, size_t size);
void rel_sha256_final(RelSha256 *s, unsigned char digest[32]);
void rel_sha256_hex(const unsigned char digest[32], char hex[65]);

#endif
