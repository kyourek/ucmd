#include <stddef.h>
#include "ucmdexample.h"

static const char *cmd(ucCmdLine *cmd, void *state) {
    int i, j;
    ucTok *arg_tok, *switch_tok;
    
    i = 0;
    arg_tok = ucCmdLine_get_arg(cmd);
    while (arg_tok) {
        ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found argument: %s", ucTok_get_value((ucTok*)arg_tok)));
        arg_tok = ucCmdLine_get_arg_x(cmd, ++i);
    }

    i = 0;
    switch_tok = ucCmdLine_get_switch(cmd);
    while (switch_tok) {
        ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found switch: %s", ucTok_get_value((ucTok*)switch_tok)));

        j = 0;
        arg_tok = ucCmdLine_get_switch_arg(cmd, switch_tok);
        while (arg_tok) {
            ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found argument: %s", ucTok_get_value((ucTok*)arg_tok)));
            arg_tok = ucCmdLine_get_switch_arg_x(cmd, switch_tok, ++j);
        }

        switch_tok = ucCmdLine_get_switch_x(cmd, ++i);
    }

    return NULL;
}

void uc_example(ucCmdLine_TransmitFunc *transmit, ucCmdLineApp_ReceiveFunc *receive) {

    ucCmdLineOpt *commands = 
        ucCmdLineOpt_create(
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

    ucCmdLineApp *app = ucCmdLineApp_create();
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(app);

    ucCmdLine_set_transmit(cmd, transmit);
    ucCmdLine_set_command_acknowledgment(cmd, "\\\\\\\\\\\\\\");
    ucCmdLine_set_response_terminator(cmd, "///////");

    ucCmdLineApp_set_receive(app, receive);

    ucCmdLineApp_run(app, commands);

    ucCmdLineApp_destroy(app);
    ucCmdLineOpt_destroy_chain(commands);
}
