#include "text.h"

#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "gfx.h"
#include "palette.h"

#define MISSING_ADVANCE 12
#define MAX_LINE_CHARS 256

static const Glyph *find_glyph(uint32_t cp)
{
    int lo = 0;
    int hi = FONT_GLYPH_COUNT - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        if (FONT_GLYPHS[mid].codepoint == cp)
            return &FONT_GLYPHS[mid];
        if (FONT_GLYPHS[mid].codepoint < cp)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return NULL;
}

/* Reads one codepoint and advances *s. Bad bytes come out as U+FFFD one at a time, which
 * keeps a damaged string drawable and always terminates. */
static uint32_t next_cp(const char **s)
{
    const unsigned char *p = (const unsigned char *)*s;
    uint32_t cp;
    int extra, i;

    if (p[0] < 0x80) {
        *s += 1;
        return p[0];
    }
    if ((p[0] & 0xE0) == 0xC0) {
        cp = p[0] & 0x1Fu;
        extra = 1;
    } else if ((p[0] & 0xF0) == 0xE0) {
        cp = p[0] & 0x0Fu;
        extra = 2;
    } else if ((p[0] & 0xF8) == 0xF0) {
        cp = p[0] & 0x07u;
        extra = 3;
    } else {
        *s += 1;
        return 0xFFFDu;
    }
    for (i = 1; i <= extra; i++) {
        if ((p[i] & 0xC0) != 0x80) {
            *s += 1;
            return 0xFFFDu;
        }
        cp = (cp << 6) | (p[i] & 0x3Fu);
    }
    *s += extra + 1;
    return cp;
}

static int advance_of(uint32_t cp)
{
    const Glyph *g = find_glyph(cp);

    return g != NULL ? g->advance : MISSING_ADVANCE;
}

static void draw_glyph(int x, int y, uint32_t cp, int color)
{
    const Glyph *g = find_glyph(cp);
    int row, col;

    if (g == NULL) {
        gfx_frame(x + 1, y + 1, 9, 10, color);
        return;
    }
    for (row = 0; row < FONT_ROWS; row++) {
        uint16_t bits = g->rows[row];

        for (col = 0; bits != 0u; col++, bits = (uint16_t)(bits << 1))
            if (bits & 0x8000u)
                gfx_pset(x + col, y + row, color);
    }
}

/* Punctuation that may not start a line, and may not end one. */
static int no_line_start(uint32_t cp)
{
    static const uint32_t LIST[] = {
        0xFF0C, 0x3002, 0xFF01, 0xFF1F, 0x3001, 0xFF1B, 0xFF1A, 0xFF09, 0x300B,
        0x300D, 0x300F, 0x201D, 0x2019, 0x2026, 0x2014, 0x3009, 0x3011, ',',
        '.',    '!',    '?',    ';',    ':',    ')',
    };
    size_t i;

    for (i = 0; i < sizeof LIST / sizeof LIST[0]; i++)
        if (LIST[i] == cp)
            return 1;
    return 0;
}

static int no_line_end(uint32_t cp)
{
    static const uint32_t LIST[] = {0xFF08, 0x300A, 0x300C, 0x300E, 0x201C, 0x2018, 0x3008, 0x3010, '('};
    size_t i;

    for (i = 0; i < sizeof LIST / sizeof LIST[0]; i++)
        if (LIST[i] == cp)
            return 1;
    return 0;
}

int text_length(const char *utf8)
{
    int n = 0;

    while (*utf8 != '\0') {
        next_cp(&utf8);
        n++;
    }
    return n;
}

int text_width(const char *utf8)
{
    int w = 0;

    while (*utf8 != '\0')
        w += advance_of(next_cp(&utf8));
    return w;
}

int text_draw(int x, int y, const char *utf8, int color)
{
    int start = x;

    while (*utf8 != '\0') {
        uint32_t cp = next_cp(&utf8);

        draw_glyph(x, y, cp, color);
        x += advance_of(cp);
    }
    return x - start;
}

/* Lays out one line starting at s into line[], applying the rules against forbidden line
 * starts and ends. Returns where the next line begins (at the newline, if one ended this). */
static const char *layout_line(const char *s, int w, uint32_t *line, int *out_count)
{
    const char *cursor = s;
    int count = 0;
    int width = 0;

    while (*cursor != '\0' && count < MAX_LINE_CHARS) {
        const char *before = cursor;
        uint32_t cp = next_cp(&cursor);
        int adv;

        if (cp == '\n') {
            cursor = before;
            break;
        }
        adv = advance_of(cp);
        if (width + adv > w && count > 0) {
            cursor = before;
            break;
        }
        line[count++] = cp;
        width += adv;
    }

    if (*cursor != '\0' && *cursor != '\n' && count > 2) {
        const char *probe = cursor;
        uint32_t following = next_cp(&probe);
        int give_back = (no_line_start(following) && !no_line_start(line[count - 1])) ||
                        no_line_end(line[count - 1]);

        if (give_back) {
            int i;

            count--;
            cursor = s;
            for (i = 0; i < count; i++)
                next_cp(&cursor);
        }
    }
    *out_count = count;
    return cursor;
}

int text_first_line(const char *utf8, int w)
{
    uint32_t line[MAX_LINE_CHARS];
    int count = 0;

    layout_line(utf8, w, line, &count);
    return count;
}

int text_box(int x, int y, int w, const char *utf8, int color, int max_chars)
{
    const char *s = utf8;
    int lines = 0;
    int drawn = 0;

    while (*s != '\0') {
        uint32_t line[MAX_LINE_CHARS];
        int count = 0;
        const char *cursor = layout_line(s, w, line, &count);

        if (color >= 0) {
            int px = x;
            int i;

            for (i = 0; i < count; i++) {
                if (max_chars >= 0 && drawn >= max_chars)
                    break;
                draw_glyph(px, y + lines * TEXT_LINE_H, line[i], color);
                px += advance_of(line[i]);
                drawn++;
            }
        }
        lines++;
        s = cursor;
        if (*s == '\n') {
            s++;
            if (*s == '\0')
                lines++;
        }
    }
    return lines;
}
