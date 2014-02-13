#include <stddef.h>
#include "uc_example.h"

static const char *cmd(uc_cmd_line *cmd, void *state) {
    uc_cmd_tok *cmd_tok;
    uc_arg_tok *arg_tok;
    uc_switch_tok *switch_tok;

    cmd_tok = uc_cmd_line_get_cmd_tok(cmd);
    
    arg_tok = uc_cmd_tok_get_arg(cmd_tok);
    while (arg_tok) {

        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Found argument: %s", uc_tok_get_value((uc_tok*)arg_tok)));
        arg_tok = uc_arg_tok_get_next(arg_tok);
    }

    switch_tok = uc_cmd_tok_get_switch(cmd_tok);
    while (switch_tok) {

        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Found switch: %s", uc_tok_get_value((uc_tok*)switch_tok)));

        arg_tok = uc_switch_tok_get_arg(switch_tok);
        while (arg_tok) {

            uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Found argument: %s", uc_tok_get_value((uc_tok*)arg_tok)));
            arg_tok = uc_arg_tok_get_next(arg_tok);
        }

        switch_tok = uc_switch_tok_get_next(switch_tok);
    }

    return NULL;
}

void uc_example(uc_cmd_line_transmit_func *transmit, uc_cmd_line_app_receive_func *receive) {

    uc_cmd_line_opt *commands = 
        uc_cmd_line_opt_create(
            cmd,
            NULL,
            "cmd",
            "A simple example command.",
            uc_arg_opt_create(
                "arg",
                "Command argument.",
            NULL),
            uc_switch_opt_create(
                "-sw1",
                "Switch 1.",
                uc_arg_opt_create(
                    "s1-a1",
                    "Switch 1, arg 1.",
                uc_arg_opt_create(
                    "s1-a2",
                    "Switch 1, arg 2.",
                NULL)),
            uc_switch_opt_create_required(
                "-sw2",
                "Switch 2.",
                uc_arg_opt_create_required(
                    "s2-a1",
                    "Switch 2, arg 1.",
                NULL),
            NULL)),
        NULL);

    uc_cmd_line_app *app = uc_cmd_line_app_get_instance();
    uc_cmd_line *cmd = uc_cmd_line_app_get_cmd(app);

    uc_cmd_line_set_transmit(cmd, transmit);
    uc_cmd_line_app_set_receive(app, receive);

    uc_cmd_line_app_run(app, commands);
}
