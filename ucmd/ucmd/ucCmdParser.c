#include "ucmd_internal.h"

ucInstance_INIT(ucCmdParser, ucCmdParser_COUNT);

static ucBool is_char_white_space(char c) {
    return (((c >= 0x09) && (c <= 0x0D)) || (c == 0x020)) 
        ? ucBool_true 
        : ucBool_false;
}

static char is_char_a_quote(char c) {
    if (c == '"') return '"';
    if (c == '\'') return '\'';
    return '\0';
}

static void remove_cmd_char(ucCmdParser *p, char *cmd, int index) {
    char done;
    assert(cmd);
    done = ucCmdParser_CMD_TERMINATOR;
    while (cmd[index] != done) {
        cmd[index] = cmd[index + 1];
        index++;
    }
}

ucCmdParser *ucCmdParser_init(ucCmdParser *p) {
    assert(p);
    return p;
}

ucCmdParser *ucCmdParser_create(void) {
    return ucCmdParser_init(ucInstance_create());
}

void ucCmdParser_destroy(ucCmdParser *p) {
    ucInstance_destroy(p);
}

ucCmdTok *ucCmdParser_parse(ucCmdParser *p, char *cmd) {
    int i, j, len;
    char done, quote, current_quote;

    assert(p);
    assert(cmd);

    /* Get the character that terminates the parse. */
    done = ucCmdParser_CMD_TERMINATOR;

    /* Get the length of the whole command. */
    len = strlen(cmd);

    /* Replace any command-terminating characters in the string with a space. */
    for (i = 0; i < len; i++) {
        if (cmd[i] == done) {
            cmd[i] = ' ';
        }
    }

    /* Append a command terminator. */
    cmd[len + 1] = done;

    /* Loop through each character in the command. */
    i = 0;
    current_quote = 0;
    while (cmd[i] != done) {

        /* Check if this command character is a quote. */
        quote = is_char_a_quote(cmd[i]);
        if (quote) {

            /* Check if this is our current quote. */
            if (quote == current_quote) {

                /* Remove the quote only if this is
                not an empty quote. */
                if ((0 < i) && (current_quote != cmd[i - 1])) {
                    cmd[i] = ' ';
                }

                /* The quoted item is finished. */
                current_quote = 0;
            }
            else {

                /* Check if we're NOT in a quote. */
                if (!current_quote) {

                    /* We've started a quote. */
                    current_quote = quote;

                    /* Remove the quote character 
                    only if this is not an empty
                    quote. */
                    if (current_quote != cmd[i + 1]) {
                        remove_cmd_char(p, cmd, i);
                    }
                }
            }
        }

        /* Check if we're not in a quoted string. */
        if (!current_quote) {

            /* Check if we're in a token separator. */
            if (is_char_white_space(cmd[i])) {

                /* Separate this token. */
                cmd[i] = '\0';

                /* Remove any remaining white space. */
                j = i + 1;
                while (is_char_white_space(cmd[j])) {
                    remove_cmd_char(p, cmd, j);
                }
            }
        }

        /* Go to the next character. */
        i++;
    }

    /* We're done parsing. */
    return (ucCmdTok*)cmd;
}
