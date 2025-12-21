#pragma once
#include <string.h>


inline void append_char(char* text, const char insert_char) {
    const int len = strlen(text);
    memmove(text + 1, text, len + 1);
    text[0] = insert_char;
}

/* Adds newlines to split a long text string into multiple lines of length < `width` */
inline void split_text(char* text, const int width) {
    const int txt_len = strlen(text);
    if (txt_len <= width)
        return;

    for (int i = width; i > 0; --i) {
        if (text[i] == ' ') {
            text[i] = '\n';
            split_text(text + i + 1, width);
            return;
        }
    }

    append_char(text + width, '\n');
    split_text(text + width + 1, width);
}