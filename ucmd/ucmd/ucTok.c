#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "ucmd_internal.h"

static ucBool is_char_digit(char c) {
    static const char *digits = "0123456789";
    const char *d;
    for (d = digits; *d; d++) {
        if (*d == c) {
            return ucBool_true;
        }
    }
    return ucBool_false;
}

int ucTok_get_length(ucTok *p) {
    int length;
    assert(p);
    for (length = 0; *p; length++, p++);
    return length;
}

const char *ucTok_get_value(ucTok *p) {
    assert(p);
    return p;
}

ucBool ucTok_equals(ucTok *p, const char *value) {
    int i, len;

    assert(p);

    /* Check if these pointers are the same. */
    if (value == p) return ucBool_true;

    /* Make sure the other arg exists. */
    if (!value) return ucBool_false;

    /* Check if the string lengths are the same. */
    len = ucTok_get_length(p);
    if (strlen(value) != len) return ucBool_false;

    /* Check for character equality. */
    for (i = 0; i < len; i++) {
        if (p[i] != value[i]) {
            return ucBool_false;
        }
    }

    /* If we got here, then the strings are equal. */
    return ucBool_true;
}

ucBool ucTok_is_integer(ucTok *p) {
    const char *str = ucTok_get_value(p);

    /* Handle NULL values. */
    if (!str) return ucBool_false;

    /* Handle negative numbers. */
    if (*str == '-') ++str;

    /* Handle empty string or just "-". */
    if (!*str) return ucBool_false;

    /* Check for non-digit chars in the rest of the stirng. */
    while (*str) {
        if (!isdigit(*str)) return ucBool_false;
        ++str;
    }

    return ucBool_true;
}

ucBool ucTok_try_parse_integer(ucTok *p, int *value) {
    if (ucTok_is_integer(p)) {
        if (value) {
            *value = atoi(ucTok_get_value(p));
        }
        return ucBool_true;
    }
    return ucBool_false;
}

int ucTok_parse_integer(ucTok *p) {
    int value = 0;
    ucTok_try_parse_integer(p, &value);
    return value;
}

ucBool ucTok_is_numeric(ucTok *p) {
    int i, len, dec_found;

    assert(p);

    /* Get the length of the token. */
    len = ucTok_get_length(p);
    
    /* Numbers need to have at least 1 character. */
    if (len < 1) return ucBool_false;
    
    /* We are allowed to start with a '-' or '.' 
    for negative numbers and decimals. We need
    more than 1 char if we do, though. */
    if ((p[0] == '-') || (p[0] == '.')) {
        if (len < 2) {
            return ucBool_false;
        }
    }

    /* Initialize vars. */
    dec_found = 0;

    /* Loop through the token. */
    for (i = 0; i < len; i++) {
        switch(p[i]) {
        
            /* Allow a dash only at the beginning. */
            case '-':
                if (i != 0) return ucBool_false;
                break;
                
            /* Allow only 1 dot. */
            case '.':
                if (dec_found) return ucBool_false;
                dec_found = 1;
                break;
                
            /* Everything else has to be a number. */
            default:
                if (!is_char_digit(p[i])) return ucBool_false;
                break;
        }
    }
    
    /* If we got here, it's a number. */
    return ucBool_true;
}

ucBool ucTok_try_parse_numeric(ucTok *p, double *value) {
    if (ucTok_is_numeric(p)) {
        if (value) {
            *value = atof(ucTok_get_value(p));
        }
        return ucBool_true;
    }
    return ucBool_false;
}

double ucTok_parse_numeric(ucTok *p) {
    double value = 0;
    ucTok_try_parse_numeric(p, &value);
    return value;
}

ucBool ucTok_is_boolean(ucTok *p) {
    static const char *b[] = { ucTok_BOOLEAN_FALSE, ucTok_BOOLEAN_TRUE };
    int i, len;

    len = sizeof(b) / sizeof(b[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, b[i])) {
            return ucBool_true;
        }
    }
    return ucBool_false;
}

ucBool ucTok_try_parse_boolean(ucTok *p, ucBool *value) {
    static const char *t[] = { ucTok_BOOLEAN_TRUE };
    static const char *f[] = { ucTok_BOOLEAN_FALSE };
    int i, len;

    len = sizeof(t) / sizeof(t[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, t[i])) {
            if (value) {
                *value = ucBool_true;
            }
            return ucBool_true;
        }
    }

    len = sizeof(f) / sizeof(f[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, f[i])) {
            if (value) {
                *value = ucBool_false;
            }
            return ucBool_true;
        }
    }
    return ucBool_false;
}

ucBool ucTok_parse_boolean(ucTok *p) {
    ucBool value = ucBool_false;
    ucTok_try_parse_boolean(p, &value);
    return value;
}

ucBool ucTok_is_switch(ucTok* p) {
    int len;

    assert(p);

    /* Get the length so we can use it. */
    len = ucTok_get_length(p);
    
    /* Check for at least 2 characters (one '-' and at least another char). */
    if (len < 2) return ucBool_false;
    
    /* Check if it starts with a '-'. */
    if (p[0] != '-') return ucBool_false;
    
    /* Check if this is a numeric argument (negative numbers aren't switches). */
    if (ucTok_is_numeric(p)) return ucBool_false;

    /* Ok, it's a switch. */
    return ucBool_true;
}

ucTok *ucTok_get_next(ucTok *p) {
    int i;
    char done;

    assert(p);

    /* Get the command terminator. */
    done = ucCmdParser_get_cmd_terminator(ucCmdParser_instance());

    /* Loop until we've hit the max length. */
    i = 0;
    while (ucTok_LENGTH_MAX > ++i) {

        /* Check if the previous character was a separator. */
        if ('\0' == p[i - 1]) {

            /* Check if this character is the terminator. */
            if (done == p[i]) {

                /* It is, so no more tokens exist. */
                return NULL;
            }

            /* Make sure this character isn't another separator. */
            if ('\0' == p[i]) {
                continue;
            }

            /* The previous character was a separator,
            and this character is NOT the terminator
            NOR another separator, so this character is 
            the start of a new token. */
            return (ucTok*)&p[i];
        }
    }

    /* The max length was exceeded,
    which probably means something
    went wrong. */
    return NULL;
}
