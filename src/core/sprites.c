#include "sprites.h"

#include <stddef.h>

#include "palette.h"

/* Slots in the art: a hair, b hair light, s skin, S skin shade, c coat, C coat dark, l coat
 * light, w linen, W linen shade, k shoes, h hat. '.' is empty. */
static const char *const ART_WOMAN_FRONT[PERSON_H] = {
    "................",
    "................",
    "......wwww......",
    ".....wwwwww.....",
    ".....wWWWWw.....",
    "......ssss......",
    "......sSSs......",
    "......ssss......",
    ".....wwssww.....",
    "....ccwwwwcc....",
    "....ccWwwWcc....",
    "...cccWwwWccc...",
    "...cC.cccc.Cc...",
    "...cC.cCCc.Cc...",
    "...ss.cCCc.ss...",
    "......wwww......",
    ".....wwwwww.....",
    ".....wWwwWw.....",
    ".....wWwwWw.....",
    "....cwWwwWwc....",
    "....cwWwwWwc....",
    "....cwWwwWwc....",
    "....cwwwwwwc....",
    "...CcwwwwwwcC...",
    "...CcWWWWWWcC...",
    "...CccccccccC...",
    "...CCcCCCCcCC...",
    "...CCCCCCCCCC...",
    ".....kk..kk.....",
    "................",
};

static const char *const ART_WOMAN_BACK[PERSON_H] = {
    "................",
    "................",
    "......wwww......",
    ".....wwwwww.....",
    ".....wWWWWw.....",
    ".....wwwwww.....",
    "......wWWw......",
    "......ssss......",
    ".....cccccc.....",
    "....cccccccc....",
    "....cccCCccc....",
    "...cccCCCCccc...",
    "...cC.cCCc.Cc...",
    "...cC.cCCc.Cc...",
    "...ss.cCCc.ss...",
    "......cccc......",
    ".....cccccc.....",
    ".....cCccCc.....",
    ".....cCccCc.....",
    "....ccCccCcc....",
    "....ccCccCcc....",
    "....ccCccCcc....",
    "....ccCccCcc....",
    "...CccCccCccC...",
    "...CccCccCccC...",
    "...CccCccCccC...",
    "...CCcCCCCcCC...",
    "...CCCCCCCCCC...",
    ".....kk..kk.....",
    "................",
};

static const char *const ART_MAN_FRONT[PERSON_H] = {
    "................",
    ".....hhhhhh.....",
    "....hhhhhhhh....",
    "..hhhhhhhhhhhh..",
    "......aaaa......",
    ".....assssa.....",
    ".....asSSsa.....",
    "......ssss......",
    ".....wwwwww.....",
    "....ccwwwwcc....",
    "...cccWwwWccc...",
    "...ccclwwlccc...",
    "...ccclwwlccc...",
    "...cCcllllcCc...",
    "...cCcclcccCc...",
    "...cCcclcccCc...",
    "...ss.cclcc.ss..",
    "......cclcc.....",
    ".....ccclccc....",
    ".....cCCcCCc....",
    ".....cC..Cc.....",
    ".....ww..ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    "....kkk..kkk....",
    "................",
};

static const char *const ART_MAN_BACK[PERSON_H] = {
    "................",
    ".....hhhhhh.....",
    "....hhhhhhhh....",
    "..hhhhhhhhhhhh..",
    "......aaaa......",
    ".....aaaaaa.....",
    ".....aabbaa.....",
    "......aaaa......",
    ".....ccwwcc.....",
    "....cccccccc....",
    "...cccccccccc...",
    "...ccccCCcccc...",
    "...ccccCCcccc...",
    "...cCccCCccCc...",
    "...cCccCCccCc...",
    "...cCccCCccCc...",
    "...ss.cCCcc.ss..",
    "......cCCcc.....",
    ".....ccCCccc....",
    ".....cCCcCCc....",
    ".....cC..Cc.....",
    ".....ww..ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    ".....wW..Ww.....",
    "....kkk..kkk....",
    "................",
};

typedef struct {
    int body;
    uint8_t hair, hair_light, skin, skin_shade, coat, coat_dark, coat_light;
    uint8_t linen, linen_shade, shoes, hat;
} Outfit;

static const Outfit OUTFITS[OUTFIT_KINDS] = {
    {BODY_WOMAN, C_EARTH + 2, C_EARTH + 3, C_SKIN + 3, C_SKIN + 2, C_SOOT + 3, C_SOOT + 2, C_SOOT + 4,
     C_LINEN + 1, C_LINEN, C_SOOT + 1, C_SOOT + 1},
    {BODY_WOMAN, C_STONE + 5, C_STONE + 6, C_SKIN + 2, C_SKIN + 1, C_RED, C_BRICK + 1, C_RED + 1,
     C_LINEN + 1, C_LINEN, C_SOOT + 1, C_SOOT + 1},
    {BODY_MAN, C_EARTH + 1, C_EARTH + 2, C_SKIN + 3, C_SKIN + 2, C_BLUE + 1, C_BLUE, C_BLUE + 3,
     C_LINEN + 2, C_LINEN, C_SOOT, C_SOOT + 1},
    {BODY_MAN, C_LINEN + 2, C_LINEN + 1, C_SKIN + 4, C_SKIN + 3, C_RED + 1, C_RED, C_LAMP + 3,
     C_LINEN + 2, C_LINEN + 1, C_SOOT, C_SOOT + 1},
    {BODY_MAN, C_EARTH + 2, C_EARTH + 3, C_SKIN_DK + 2, C_SKIN_DK + 1, C_WOOD + 3, C_WOOD + 1, C_WOOD + 4,
     C_LINEN, C_STONE + 4, C_WOOD, C_WOOD + 1},
};

static uint8_t g_person[OUTFIT_KINDS][2][PERSON_W * PERSON_H];

static int slot(const Outfit *o, char ch)
{
    switch (ch) {
    case '.': return 0;
    case 'a': return o->hair;
    case 'b': return o->hair_light;
    case 's': return o->skin;
    case 'S': return o->skin_shade;
    case 'c': return o->coat;
    case 'C': return o->coat_dark;
    case 'l': return o->coat_light;
    case 'w': return o->linen;
    case 'W': return o->linen_shade;
    case 'k': return o->shoes;
    case 'h': return o->hat;
    default: return -1;
    }
}

static int bake_one(const char *const art[PERSON_H], const Outfit *o, uint8_t *out)
{
    int x, y;

    for (y = 0; y < PERSON_H; y++) {
        for (x = 0; x < PERSON_W; x++) {
            int c;

            if (art[y][x] == '\0')
                return y + 1;
            c = slot(o, art[y][x]);
            if (c < 0)
                return y + 1;
            out[y * PERSON_W + x] = (uint8_t)c;
        }
        if (art[y][PERSON_W] != '\0')
            return y + 1;
    }
    return 0;
}

int sprites_bake(void)
{
    int i;

    for (i = 0; i < OUTFIT_KINDS; i++) {
        const Outfit *o = &OUTFITS[i];
        const char *const *front = o->body == BODY_WOMAN ? ART_WOMAN_FRONT : ART_MAN_FRONT;
        const char *const *back = o->body == BODY_WOMAN ? ART_WOMAN_BACK : ART_MAN_BACK;
        int bad = bake_one(front, o, g_person[i][0]);

        if (bad == 0)
            bad = bake_one(back, o, g_person[i][1]);
        if (bad != 0)
            return bad;
    }
    return 0;
}

const uint8_t *sprite_person(int outfit, int back)
{
    if (outfit < 0 || outfit >= OUTFIT_KINDS)
        return NULL;
    return g_person[outfit][back ? 1 : 0];
}

int sprite_outfit_body(int outfit)
{
    return OUTFITS[outfit].body;
}
