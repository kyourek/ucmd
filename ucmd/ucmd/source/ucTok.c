#include <string.h>
#include "ucTok_p.h"

const char ucTok_separator = '\0';
const char uc_cmd_terminator = '\n';

static ucBool is_char_digit(char c) {
    /* The native isdigit function in ctype.h was giving some weird behavior in the uVision simulator */
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
    if (NULL == p) return 0;

    for (length = 0; *p != ucTok_separator; length++, p++);

    return length;
}

const char *ucTok_get_value(ucTok *p) {
    if (NULL == p) return NULL;
    return (const char*)p;
}

ucBool ucTok_equals(ucTok *p, const char *value) {
    int i, len;

    /* check if these pointers are the same */
    if (value == p) return ucBool_true;

    /* make sure neither arg is null */
    if (NULL == p) return ucBool_false;
    if (NULL == value) return ucBool_false;

    /* check if the string lengths are the same */
    len = ucTok_get_length(p);
    if (strlen(value) != len) return ucBool_false;

    /* check for character equality */
    for (i = 0; i < len; i++) {
        if (p[i] != value[i]) {
            return ucBool_false;
        }
    }

    /* if we got here, then the strings are equal */
    return ucBool_true;
}

ucBool ucTok_is_numeric(ucTok *p) {
    int i, len;
    ucBool dec_found;

    /* check if p is NULL */
    if (NULL == p) return ucBool_false;
    
    /* get the length of the string */
    len = ucTok_get_length(p);
    
    /* numbers need to have at least 1 character */
    if (len < 1) return ucBool_false;
    
    /* we are allowed to start with a '-' or '.' for negative numbers and decimals */
    if ((p[0] == '-') || (p[0] == '.')) {
        
        /* but we need more than 1 char if we do */
        if (len < 2) return ucBool_false;
    }

    // initialize vars
    dec_found = ucBool_false;

    /* loop through the chars */
    for (i = 0; i < len; i++) {
        
        switch(p[i]) {
        
            /* allow a dash only at the beginning */
            case '-':
                if (i != 0) return ucBool_false;
                break;
                
            /* allow only 1 dot */
            case '.':
                if (dec_found) return ucBool_false;
                dec_found = ucBool_true;
                break;
                
            /* everything else has to be a number */
            default:
                if (is_char_digit(p[i]) == ucBool_false) return ucBool_false;
                break;
        }
    }
    
    /* if we got here, it's a number */
    return ucBool_true;
}

ucBool ucTok_is_switch(ucTok* p) {
    int len = 0;

    /* check for a null pointer */
    if (NULL == p) return ucBool_false;
    
    /* get the length so we can use it */
    len = ucTok_get_length(p);
    
    /* check for at least 2 characters (one '-' and at least another char) */
    if (len < 2) return ucBool_false;
    
    /* check if it starts with a '-' */
    if (p[0] != '-') return ucBool_false;
    
    /* check if this is a numeric argument (negative numbers aren't switches) */
    if (ucTok_is_numeric(p)) return ucBool_false;

    /* ok, it's a switch */
    return ucBool_true;
}

ucTok *ucTok_get_next(ucTok *p) {
    int i;
    if (NULL == p) return NULL;

    /* loop until we've hit the max length */
    i = 0;
    while (ucTok_LENGTH_MAX > ++i) {

        /* check if the previous character was a separator */
        if (ucTok_separator == p[i - 1]) {

            /* check if this character is the terminator */
            if (uc_cmd_terminator == p[i]) {

                /* it is, so no more tokens exist */
                return NULL;
            }

            /* make sure this character isn't another separator */
            if (ucTok_separator == p[i]) {
                continue;
            }

            /* the previous character was a separator,
               and this character is NOT the terminator,
               OR another separator, so this character is 
               the start of a new token */
            return (ucTok*)&p[i];
        }
    }

    /* the max length was exceeded,
       which probably means something
       went wrong. */
    return NULL;
}

int ucTok_count(ucTok *p) {
    int count;

    /* make sure we have a pointer */
    if (NULL == p) return 0;

    /* if the first character is the terminator, then we have no tokens */
    if (p[0] == uc_cmd_terminator) return 0;

    /* start off the count according to whether or not
       we're starting at a token */
    count = ucTok_separator == p[0] ? -1 : 0;

    /* count the tokens in the chain */
    while (NULL != p) {
        count++;
        p = ucTok_get_next(p);
    }
    return count;
}
