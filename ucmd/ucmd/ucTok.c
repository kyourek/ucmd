#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "ucmd_internal.h"

static ucBool is_char_digit(char c) {
    /* The native isdigit function in ctype.h was giving some weird behavior in the uVision simulator. */
    static const char *digits = "0123456789";
    const char *d;
    for (d = digits; *d; d++) {
        if (*d == c) {
            return ucBool_TRUE;
        }
    }
    return ucBool_FALSE;
}

int ucTok_get_length(ucTok *p) {
    int length;
    if (NULL == p) return 0;

    for (length = 0; *p; length++, p++);

    return length;
}

const char *ucTok_get_value(ucTok *p) {
    if (NULL == p) return NULL;
    return (const char*)p;
}

ucBool ucTok_equals(ucTok *p, const char *value) {
    int i, len;

    /* check if these pointers are the same */
    if (value == p) return ucBool_TRUE;

    /* make sure neither arg is null */
    if (NULL == p) return ucBool_FALSE;
    if (NULL == value) return ucBool_FALSE;

    /* check if the string lengths are the same */
    len = ucTok_get_length(p);
    if (strlen(value) != len) return ucBool_FALSE;

    /* check for character equality */
    for (i = 0; i < len; i++) {
        if (p[i] != value[i]) {
            return ucBool_FALSE;
        }
    }

    /* if we got here, then the strings are equal */
    return ucBool_TRUE;
}

ucBool ucTok_is_integer(ucTok *p) {
    const char *str = ucTok_get_value(p);

    /* Handle NULL values. */
    if (NULL == str) return ucBool_FALSE;

    /* Handle negative numbers. */
    if (*str == '-') ++str;

    /* Handle empty string or just "-". */
    if (!*str) return ucBool_FALSE;

    /* Check for non-digit chars in the rest of the stirng. */
    while (*str) {
        if (!isdigit(*str)) return ucBool_FALSE;
        ++str;
    }

    return ucBool_TRUE;
}

ucBool ucTok_try_parse_integer(ucTok *p, int *value) {
    if (ucTok_is_integer(p)) {
        if (value) {
            *value = atoi(ucTok_get_value(p));
        }
        return ucBool_TRUE;
    }
    return ucBool_FALSE;
}

int ucTok_parse_integer(ucTok *p) {
    int value = 0;
    ucTok_try_parse_integer(p, &value);
    return value;
}

ucBool ucTok_is_numeric(ucTok *p) {
    int i, len;
    ucBool dec_found;

    /* check if p is NULL */
    if (NULL == p) return ucBool_FALSE;
    
    /* get the length of the string */
    len = ucTok_get_length(p);
    
    /* numbers need to have at least 1 character */
    if (len < 1) return ucBool_FALSE;
    
    /* we are allowed to start with a '-' or '.' for negative numbers and decimals */
    if ((p[0] == '-') || (p[0] == '.')) {
        
        /* but we need more than 1 char if we do */
        if (len < 2) return ucBool_FALSE;
    }

    // initialize vars
    dec_found = ucBool_FALSE;

    /* loop through the chars */
    for (i = 0; i < len; i++) {
        
        switch(p[i]) {
        
            /* allow a dash only at the beginning */
            case '-':
                if (i != 0) return ucBool_FALSE;
                break;
                
            /* allow only 1 dot */
            case '.':
                if (dec_found) return ucBool_FALSE;
                dec_found = ucBool_TRUE;
                break;
                
            /* everything else has to be a number */
            default:
                if (is_char_digit(p[i]) == ucBool_FALSE) return ucBool_FALSE;
                break;
        }
    }
    
    /* if we got here, it's a number */
    return ucBool_TRUE;
}

ucBool ucTok_try_parse_numeric(ucTok *p, double *value) {
    if (ucTok_is_numeric(p)) {
        if (value) {
            *value = atof(ucTok_get_value(p));
        }
        return ucBool_TRUE;
    }
    return ucBool_FALSE;
}

double ucTok_parse_numeric(ucTok *p) {
    double value = 0;
    ucTok_try_parse_numeric(p, &value);
    return value;
}

ucBool ucTok_is_boolean(ucTok *p) {
    int i, len;
    const char *b[] = { ucTok_BOOLEAN_FALSE, ucTok_BOOLEAN_TRUE };
    if (NULL == p) return ucBool_FALSE;
    len = sizeof(b) / sizeof(b[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, b[i])) {
            return ucBool_TRUE;
        }
    }
    return ucBool_FALSE;
}

ucBool ucTok_try_parse_boolean(ucTok *p, ucBool *value) {
    int i, len;
    const char *t[] = { ucTok_BOOLEAN_TRUE };
    const char *f[] = { ucTok_BOOLEAN_FALSE };
    len = sizeof(t) / sizeof(t[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, t[i])) {
            if (value) {
                *value = ucBool_TRUE;
            }
            return ucBool_TRUE;
        }
    }
    len = sizeof(f) / sizeof(f[0]);
    for (i = 0; i < len; i++) {
        if (ucTok_equals(p, f[i])) {
            if (value) {
                *value = ucBool_FALSE;
            }
            return ucBool_TRUE;
        }
    }
    return ucBool_FALSE;
}

ucBool ucTok_parse_boolean(ucTok *p) {
    ucBool value = ucBool_FALSE;
    ucTok_try_parse_boolean(p, &value);
    return value;
}

ucBool ucTok_is_switch(ucTok* p) {
    int len = 0;

    /* check for a null pointer */
    if (NULL == p) return ucBool_FALSE;
    
    /* get the length so we can use it */
    len = ucTok_get_length(p);
    
    /* check for at least 2 characters (one '-' and at least another char) */
    if (len < 2) return ucBool_FALSE;
    
    /* check if it starts with a '-' */
    if (p[0] != '-') return ucBool_FALSE;
    
    /* check if this is a numeric argument (negative numbers aren't switches) */
    if (ucTok_is_numeric(p)) return ucBool_FALSE;

    /* ok, it's a switch */
    return ucBool_TRUE;
}

ucTok *ucTok_get_next(ucTok *p) {
    int i;

    assert(p);

    /* Loop until we've hit the max length. */
    i = 0;
    while (ucTok_LENGTH_MAX > ++i) {

        /* Check if the previous character was a separator. */
        if ('\0' == p[i - 1]) {

            /* Check if this character is the terminator. */
            if ('\n' == p[i]) {

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
