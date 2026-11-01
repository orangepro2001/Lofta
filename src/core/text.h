/* UTF-8 text on the framebuffer, with Chinese line-breaking rules. */
#ifndef LOFTA_TEXT_H
#define LOFTA_TEXT_H

#define TEXT_LINE_H 14

int text_width(const char *utf8);
/* Draws one line. Newlines are not interpreted. Returns the width drawn. */
int text_draw(int x, int y, const char *utf8, int color);
/* Wraps to width w; at most max_chars codepoints are drawn (negative: all). Returns the
 * number of lines the whole text takes, so a caller can measure with color < 0. */
int text_box(int x, int y, int w, const char *utf8, int color, int max_chars);
/* Codepoints on the first line of utf8 wrapped to width w. */
int text_first_line(const char *utf8, int w);
/* Codepoints in a string. */
int text_length(const char *utf8);

#endif
