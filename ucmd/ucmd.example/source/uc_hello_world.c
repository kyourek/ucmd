#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "uc_cmd_line_app.h"
#include "uc_hello_world.h"

#define NAME_LENGTH 50

struct app_state {
    char first_name[NAME_LENGTH];
    char last_name[NAME_LENGTH];
    char full_name[NAME_LENGTH * 2 + 1];
};

typedef struct app_state app_state;

static const char *format_name(app_state *state) {
    int i, j, len_first, len_last;
    const char *last, *first;
    static const char *def = "World";

    if (!state) return def;

    first = state->first_name;
    last = state->last_name;

    len_first = strlen(first);
    len_last = strlen(last);

    if ((!len_first) && (!len_last)) return def;

    if (!len_first) return last;
    if (!len_last) return first;

    for (i = 0; i < len_first; i++) {
        state->full_name[i] = first[i];
    }
    
    state->full_name[i++] = ' ';

    for (j = 0; j < len_last; j++) {
        state->full_name[i + j] = last[j];
    }

    state->full_name[i + j] = '\0';

    return state->full_name;
}

static void copy_name(const char *source, char *dest) {
    int i, len;

    if (!source) {
        dest[0] = '\0';
        return;
    }

    len = NAME_LENGTH - 1;
    for (i = 0; i < len; i++) {
        if (!source[i]) break;
        dest[i] = source[i];
    }

    dest[i] = '\0';
}

static const char *name(uc_cmd_line *cmd, void *state) {

    app_state *my_state;
    uc_cmd_tok *cmd_tok;
    uc_arg_tok *name_arg;
    uc_switch_tok *root_switch, *first_name_switch, *last_name_switch;

    my_state = (app_state*)state;
    if (!my_state) return "Oops... NULL pointer!";

    cmd_tok = uc_cmd_line_get_cmd_tok(cmd);
    root_switch = uc_cmd_tok_get_switch(cmd_tok);

    first_name_switch = uc_switch_tok_find(root_switch, "-first");
    if (first_name_switch) {

        name_arg = uc_switch_tok_get_arg(first_name_switch);
        copy_name(uc_tok_get_value(name_arg), my_state->first_name);
    }

    last_name_switch = uc_switch_tok_find(root_switch, "-last");
    if (last_name_switch) {

        name_arg = uc_switch_tok_get_arg(last_name_switch);
        copy_name(uc_tok_get_value(name_arg), my_state->last_name);
    }

    return uc_cmd_line_format_response(
        cmd, 
        "Name: %s",
        format_name(my_state)
    );
}

static const char *say(uc_cmd_line *cmd, void *state) {
    app_state *my_state = (struct app_state*)state;

    uc_cmd_tok *cmd_tok = uc_cmd_line_get_cmd_tok(cmd);
    uc_arg_tok *arg_tok = uc_cmd_tok_get_arg(cmd_tok);

    const char *phrase = uc_tok_get_value(arg_tok);

    uc_cmd_line_respond(cmd, uc_cmd_line_format_response(
        cmd,
        "%s, %s!",
        phrase,
        format_name(my_state)
    ));

    return NULL;
}

void uc_hello_world(uc_cmd_line_transmit_func *transmit, uc_cmd_line_app_receive_func *receive) {
    
    app_state my_state = { 0 };

    uc_cmd_line_opt *options = 
        uc_cmd_line_opt_create(
            name,
            &my_state,
            "name",
            "Sets the user's name.",
            NULL,
            uc_switch_opt_create(
                "-first",
                "Sets the user's first name.",
                uc_arg_opt_create_required(
                    "first-name",
                    "The first-name of the user.",
                NULL),
            uc_switch_opt_create(
                "-last",
                "Sets the user's last name.",
                uc_arg_opt_create_required(
                    "last-name",
                    "The last-name of the user.",
                NULL),
            NULL)),
        uc_cmd_line_opt_create(
            say,
            &my_state,
            "say",
            "Says a phrase.",
            uc_arg_opt_create_required(
                "phrase",
                "The phrase to say.",
                NULL),
            NULL,
        NULL));

    uc_cmd_line_app *app = uc_cmd_line_app_get_instance();
    uc_cmd_line *cmd = uc_cmd_line_app_get_cmd(app);

    uc_cmd_line_set_transmit(cmd, transmit);
    uc_cmd_line_app_set_receive(app, receive);

    uc_cmd_line_app_run(app, options);
}