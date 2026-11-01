/* Sprites drawn as character art in source and baked to palette indices at start-up. */
#ifndef LOFTA_SPRITES_H
#define LOFTA_SPRITES_H

#include <stdint.h>

enum { BODY_WOMAN, BODY_MAN, BODY_KINDS };
enum { OUTFIT_SEARCHER, OUTFIT_MATRON, OUTFIT_CONSTABLE, OUTFIT_GENTLEMAN, OUTFIT_LABOURER, OUTFIT_KINDS };

#define PERSON_W 16
#define PERSON_H 30

/* Returns 0 on success, or the line number + 1 of the first art line with a bad character,
 * so a typo in the art is a test failure rather than a stray pixel. */
int sprites_bake(void);

/* Back view when back is nonzero. */
const uint8_t *sprite_person(int outfit, int back);
int sprite_outfit_body(int outfit);

#endif
