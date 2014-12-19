#include <string.h>
#include "ucCmdParser_p.h"
#include "ucCmdTok.h"
#include "ucTok_p.h"

static ucBool is_char_white_space(char c) {
    /* The native isspace function in ctype.h was giving some weird behavior in the uVision simulator. */
    return (((c >= 0x09) && (c <= 0x0D)) || (c == 0x020)) ? ucBool_true : ucBool_false;
}

static char is_char_a_quote(char c) {
    if (c == '"') return '"';
    if (c == '\'') return '\'';
    return 0;
}

static void remove_cmd_char(char *cmd, int index) {
    if (NULL == cmd) return;
    while (cmd[index] != uc_cmd_terminator) {
        cmd[index] = cmd[index + 1];
        index++;
    }
}

static ucCmdTok *parse(ucCmdParser *p, char *cmd) {
    int i, j, len;
    char quote, current_quote;

    if (NULL == cmd) return NULL;

    /* get the length of the whole command */
    len = strlen(cmd);

    /* replace any command-terminating characters in the string with a space */
    for (i = 0; i < len; i++) {
        if (cmd[i] == uc_cmd_terminator) {
            cmd[i] = ' ';
        }
    }

    /* append a command terminator */
    cmd[len + 1] = uc_cmd_terminator;

    /* loop through each character in the command */
    i = 0;
    current_quote = 0;
    while (cmd[i] != uc_cmd_terminator) {

        /* check if this command character is a quote */
        quote = is_char_a_quote(cmd[i]);
        if (quote) {

            /* check if this is our current quote */
            if (quote == current_quote) {

                /* remove the quote only if this is
                   not an empty quote */
                if ((0 < i) && (current_quote != cmd[i - 1])) {
                    cmd[i] = ' ';
                }

                /* the quoted item is finished */
                current_quote = 0;
            }
            else {

                /* check if we're NOT in a quote */
                if (!current_quote) {

                    /* we've started a quote */
                    current_quote = quote;

                    /* remove the quote character 
                       only if this is not an empty
                       quote */
                    if (current_quote != cmd[i + 1]) {
                        remove_cmd_char(cmd, i);
                    }
                }
            }
        }

        /* check if we're not in a quoted string */
        if (!current_quote) {

            /* check if we're in a token separator */
            if (is_char_white_space(cmd[i])) {

                /* separate this token */
                cmd[i] = ucTok_separator;

                /* remove any remaining white space */
                j = i + 1;
                while (is_char_white_space(cmd[j])) {
                    remove_cmd_char(cmd, j);
                }
            }
        }

        /* go to the next character */
        i++;
    }

    /* we're done parsing */
    return (ucCmdTok*)cmd;
}

ucCmdTok *ucCmdParser_parse(ucCmdParser *p, char *cmd) {
    if (NULL == p) return NULL;
    if (NULL == p->parse) return NULL;
    return p->parse(p, cmd);
}

ucCmdParser *ucCmdParser_get_instance(void) {
    static ucCmdParser instance;
    static ucCmdParser *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->parse = parse;
    }
    return p;
}
