#include <ctype.h>
#include <stdlib.h>
#include "ucmd_internal.h"

int ucTok_get_length(ucTok *p) {
    int length;
    assert(p);
    for (length = 0; *p; length++, p++);
    return length;
}

ucBool ucTok_equals(ucTok *p, const char *value) {
    assert(p);
    return uc_STR_EQ(p, value);
}

ucBool ucTok_is_integer(ucTok *p) {
    assert(p);

    /* Handle negative numbers. */
    if (*p == '-') ++p;

    /* Handle empty string or just "-". */
    if (!*p) return ucBool_false;

    /* Check for non-digit chars in the rest of the string. */
    while (*p) {
        if (!isdigit((int)*p)) return ucBool_false;
        ++p;
    }

    return ucBool_true;
}

ucBool ucTok_try_parse_integer(ucTok *p, int *value) {
    if (ucTok_is_integer(p)) {
        if (value) {
            *value = atoi(p);
        }
        return ucBool_true;
    }
    return ucBool_false;
}

int ucTok_parse_integer(ucTok *p, int default_value) {
    ucTok_try_parse_integer(p, &default_value);
    return default_value;
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
                if (!isdigit((int)p[i])) return ucBool_false;
                break;
        }
    }
    
    /* If we got here, it's a number. */
    return ucBool_true;
}

ucBool ucTok_try_parse_numeric(ucTok *p, double *value) {
    if (ucTok_is_numeric(p)) {
        if (value) {
            *value = atof(p);
        }
        return ucBool_true;
    }
    return ucBool_false;
}

double ucTok_parse_numeric(ucTok *p, double default_value) {
    ucTok_try_parse_numeric(p, &default_value);
    return default_value;
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

ucBool ucTok_parse_boolean(ucTok *p, ucBool default_value) {
    ucTok_try_parse_boolean(p, &default_value);
    return default_value;
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
    done = ucParser_CMD_TERMINATOR;

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
