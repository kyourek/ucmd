#include <stddef.h>
#include "ucmdexample.h"

static const char *cmd(ucCmd *cmd, void *state) {
    int i, j;
    ucTok *command_tok, *arg_tok, *switch_tok;
    
    command_tok = ucCmd_get_command(cmd);
    ucCmd_respond(cmd, ucCmd_format_response(cmd, "Found command: %s", command_tok));

    i = 0;
    arg_tok = ucCmd_get_arg(cmd);
    while (arg_tok) {
        ucCmd_respond(cmd, ucCmd_format_response(cmd, "Found argument: %s", arg_tok));
        arg_tok = ucCmd_get_arg_x(cmd, ++i);
    }

    i = 0;
    switch_tok = ucCmd_get_switch(cmd);
    while (switch_tok) {
        ucCmd_respond(cmd, ucCmd_format_response(cmd, "Found switch: %s", switch_tok));

        j = 0;
        arg_tok = ucCmd_get_switch_arg(cmd, switch_tok);
        while (arg_tok) {
            ucCmd_respond(cmd, ucCmd_format_response(cmd, "Found argument: %s", arg_tok));
            arg_tok = ucCmd_get_switch_arg_x(cmd, switch_tok, ++j);
        }

        switch_tok = ucCmd_get_switch_x(cmd, ++i);
    }

    return NULL;
}

void uc_example(ucCmd_TransmitFunc *transmit, ucCmd_ReceiveFunc *receive) {

    ucCmdOpt *commands = 
        ucCmdOpt_create(
            cmd,
            NULL,
            "cmd",
            "A simple example command.",
            ucArgOpt_create(
                "arg",
                "Help text for the command argument.",
            NULL),
            ucSwitchOpt_create(
                "-sw1",
                "This is Switch 1.",
                ucArgOpt_create(
                    "s1-a1",
                    "Info about switch 1, arg 1.",
                ucArgOpt_create(
                    "s1-a2",
                    "Some help for switch 1, arg 2.",
                NULL)),
            ucSwitchOpt_create_required(
                "-sw2",
                "And this is the second switch, which is required.",
                ucArgOpt_create_required(
                    "s2-a1",
                    "It has a required argument.",
                NULL),
            NULL)),
        NULL);

    ucApp *app = ucApp_create();
    ucCmd *cmd = ucApp_get_cmd(app);

    ucCmd_set_transmit(cmd, transmit);
    ucCmd_set_receive(cmd, receive);
    ucCmd_set_command_acknowledgment(cmd, "\\\\\\\\\\\\\\");
    ucCmd_set_response_terminator(cmd, "///////");

    ucApp_run(app, commands);

    ucApp_destroy(app);
    ucCmdOpt_destroy_chain(commands);
}
