#include <stddef.h>
#include "uc_example.h"

static const char *cmd(ucCmdLine *cmd, void *state) {
    ucCmdTok *cmd_tok;
    ucArgTok *arg_tok;
    ucSwitchTok *switch_tok;

    cmd_tok = ucCmdLine_get_cmd_tok(cmd);
    
    arg_tok = ucCmdTok_get_arg(cmd_tok);
    while (arg_tok) {

        ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found argument: %s", ucTok_get_value((ucTok*)arg_tok)));
        arg_tok = ucArgTok_get_next(arg_tok);
    }

    switch_tok = ucCmdTok_get_switch(cmd_tok);
    while (switch_tok) {

        ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found switch: %s", ucTok_get_value((ucTok*)switch_tok)));

        arg_tok = ucSwitchTok_get_arg(switch_tok);
        while (arg_tok) {

            ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Found argument: %s", ucTok_get_value((ucTok*)arg_tok)));
            arg_tok = ucArgTok_get_next(arg_tok);
        }

        switch_tok = ucSwitchTok_get_next(switch_tok);
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

    ucCmdLineApp *app = ucCmdLineApp_get_instance();
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(app);

    ucCmdLine_set_transmit(cmd, transmit);
    ucCmdLineApp_set_receive(app, receive);
    ucCmdLineApp_set_response_terminator(app, "*******");

    ucCmdLineApp_run(app, commands);
}
