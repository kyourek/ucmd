#include <stdlib.h>
#include <string.h>
#include "cl_common.h"
#include "cl_tok.h"
#include "cl_tok_p.h"

const char cl_tok_separator = '\0';
const char cl_cmd_terminator = '\n';

static CL_BOOL is_char_digit(char c) {
    /* The native isdigit function in ctype.h was giving some weird behavior in the uVision simulator */
    static char *digits = "0123456789";
    char *d;
    for (d = digits; *d; d++) {
        if (*d == c) {
            return CL_TRUE;
        }
    }
    return CL_FALSE;
}

size_t cl_tok_get_length(cl_tok *p) {
    size_t length;
    if (NULL == p) return 0;

    for (length = 0; *p != cl_tok_separator; length++, p++);

    return length;
}

const char *cl_tok_get_value(cl_tok *p) {
    if (NULL == p) return NULL;
    return (const char*)p;
}

CL_BOOL cl_tok_equals(cl_tok *p, const char *value) {
    size_t i, len;

    /* check if these pointers are the same */
    if (value == p) return CL_TRUE;

    /* make sure neither arg is null */
    if (NULL == p) return CL_FALSE;
    if (NULL == value) return CL_FALSE;

    /* check if the string lengths are the same */
    len = cl_tok_get_length(p);
    if (strlen(value) != len) return CL_FALSE;

    /* check for character equality */
    for (i = 0; i < len; i++) {
        if (p[i] != value[i]) {
            return CL_FALSE;
        }
    }

    /* if we got here, then the strings are equal */
    return CL_TRUE;
}

CL_BOOL cl_tok_is_numeric(cl_tok *p) {
    size_t i, len;
    CL_BOOL dec_found;

    /* check if p is NULL */
    if (NULL == p) return CL_FALSE;
	
    /* get the length of the string */
    len = cl_tok_get_length(p);
	
    /* numbers need to have at least 1 character */
    if (len < 1) return CL_FALSE;
	
    /* we are allowed to start with a '-' or '.' for negative numbers and decimals */
    if ((p[0] == '-') || (p[0] == '.')) {
        
        /* but we need more than 1 char if we do */
        if (len < 2) return CL_FALSE;
    }

    // initialize vars
    dec_found = CL_FALSE;

    /* loop through the chars */
    for (i = 0; i < len; i++) {
        
        switch(p[i]) {
        
            /* allow a dash only at the beginning */
            case '-':
                if (i != 0) return CL_FALSE;
                break;
				
            /* allow only 1 dot */
            case '.':
                if (dec_found) return CL_FALSE;
                dec_found = CL_TRUE;
                break;
				
            /* everything else has to be a number */
            default:
                if (is_char_digit(p[i]) == CL_FALSE) return CL_FALSE;
                break;
        }
    }
    
    /* if we got here, it's a number */
    return CL_TRUE;
}

CL_BOOL cl_tok_is_switch(cl_tok* p) {
    size_t len = 0;

    /* check for a null pointer */
    if (NULL == p) return CL_FALSE;
	
    /* get the length so we can use it */
    len = cl_tok_get_length(p);
	
    /* check for at least 2 characters (one '-' and at least another char) */
    if (len < 2) return CL_FALSE;
	
    /* check if it starts with a '-' */
    if (p[0] != '-') return CL_FALSE;
	
    /* check if this is a numeric argument (negative numbers aren't switches) */
    if (cl_tok_is_numeric(p)) return CL_FALSE;

    /* ok, it's a switch */
    return CL_TRUE;
}

cl_tok *cl_tok_get_next(cl_tok *p) {
    int i;
    if (NULL == p) return NULL;

    /* loop until we've hit the max length */
    i = 0;
    while (CL_TOK_LEN_MAX > ++i) {

        /* check if the previous character was a separator */
        if (cl_tok_separator == p[i - 1]) {

            /* check if this character is the terminator */
            if (cl_cmd_terminator == p[i]) {

                /* it is, so no more tokens exist */
                return NULL;
            }

            /* make sure this character isn't another separator */
            if (cl_tok_separator == p[i]) {
                continue;
            }

            /* the previous character was a separator,
               and this character is NOT the terminator,
               OR another separator, so this character is 
               the start of a new token */
            return (cl_tok*)&p[i];
        }
    }

    /* the max length was exceeded,
       which probably means something
       went wrong. */
    return NULL;
}

int cl_tok_count(cl_tok *p) {
    int count;

    /* make sure we have a pointer */
    if (NULL == p) return 0;

    /* if the first character is the terminator, then we have no tokens */
    if (p[0] == cl_cmd_terminator) return 0;

    /* start off the count according to whether or not
       we're starting at a token */
    count = cl_tok_separator == p[0] ? -1 : 0;

    /* count the tokens in the chain */
    while (NULL != p) {
        count++;
        p = cl_tok_get_next(p);
    }
    return count;
}
