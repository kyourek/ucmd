#include "ucmd_internal.h"

ucInstance_INIT(ucCmdApp, ucCmdApp_COUNT);

typedef struct {
    ucCmdApp *app;
    ucCmdOpt *cmd_opt;
} HelpState;

typedef struct {
    ucCmdApp *app;
} QuitState;

static const char *quit(ucCmd *cmd, void *state) {
    QuitState *s = (QuitState*)state;
    assert(s);
    return ucCmdApp_get_escape_response(s->app);
}

static const char *help(ucCmd *cmd, void *state) {
    HelpState *s = (HelpState*)state;
    ucArgTok *arg_tok;
    ucCmdOpt *cmd_opt;

    assert(s);

    cmd_opt = s->cmd_opt;
    arg_tok = ucCmdTok_get_arg(ucCmd_get_cmd_tok(cmd));
    if (arg_tok) {
        cmd_opt = ucCmdOpt_find_by_name(cmd_opt, arg_tok);
        if (cmd_opt) {
            ucCmdOpt_send_help(cmd_opt, cmd);
            return NULL;
        }
        return ucCmd_format_response(
            cmd, 
            ucOpt_INVALID "No command option found for \"%s\".", 
            arg_tok);
    }

    ucCmd_respond(cmd, "Commands");

    while (cmd_opt) {
        ucOpt_send_help((ucOpt*)cmd_opt, cmd, "\t");
        cmd_opt = ucCmdOpt_get_next(cmd_opt);
    }

    return ucCmd_format_response(
        cmd, 
        "Type \"%s\" followed by a command name for additional help with that command.", 
        ucCmdApp_get_help_command(s->app));
}

void ucCmdApp_set_escape_response(ucCmdApp *p, const char *value) {
    assert(p);
    p->escape_response = value;
}

const char *ucCmdApp_get_escape_response(ucCmdApp *p) {
    assert(p);
    return p->escape_response;
}

void ucCmdApp_set_receive(ucCmdApp *p, ucCmdApp_ReceiveFunc *value) {
    assert(p);
    p->receive = value;
}

ucCmdApp_ReceiveFunc *ucCmdApp_get_receive(ucCmdApp *p) {
    assert(p);
    return p->receive;
}

void *ucCmdApp_get_receive_state(ucCmdApp *p) {
    assert(p);
    return p->receive_state;
}

void ucCmdApp_set_receive_state(ucCmdApp *p, void *value) {
    assert(p);
    p->receive_state = value;
}

void ucCmdApp_set_quit_command(ucCmdApp *p, const char *value) {
    assert(p);
    p->quit_command = value;
}

const char *ucCmdApp_get_quit_command(ucCmdApp *p) {
    assert(p);
    return p->quit_command;
}

void ucCmdApp_set_help_command(ucCmdApp *p, const char *value) {
    assert(p);
    p->help_command = value;
}

const char *ucCmdApp_get_help_command(ucCmdApp *p) {
    assert(p);
    return p->help_command;
}

ucCmd *ucCmdApp_get_cmd(ucCmdApp *p) {
    assert(p);
    return p->cmd;
}

ucCmdParser *ucCmdApp_get_cmd_parser(ucCmdApp *p) {
    assert(p);
    return p->cmd_parser;
}

ucCmdApp *ucCmdApp_init(ucCmdApp *p, ucCmdParser *cmd_parser, ucCmd *cmd) {
    assert(p);
    p->cmd = cmd;
    p->cmd_parser = cmd_parser;
    p->receive = NULL;
    p->receive_state = NULL;
    p->help_command = "help";
    p->quit_command = "quit";
    p->escape_response = "\x1b";
    return p;
}

ucCmdApp *ucCmdApp_create(void) {
    return ucCmdApp_init(
        ucInstance_create(),
        ucCmdParser_create(),
        ucCmd_create());
}

void ucCmdApp_destroy(ucCmdApp *p) {
    if (p) {
        ucCmd_destroy(p->cmd);
        ucCmdParser_destroy(p->cmd_parser);
    }
    ucInstance_destroy(p);
}

char *ucCmdApp_receive(ucCmdApp *p) {
    assert(p);
    if (p->receive) {
        return p->receive(p->cmd_str, sizeof(p->cmd_str), p->receive_state);
    }
    return NULL;
}

size_t ucCmdApp_get_cmd_str_size_max(ucCmdApp *p) {
    assert(p);
    return (sizeof(p->cmd_str) / sizeof(p->cmd_str[0])) - 1;
}

void ucCmdApp_run(ucCmdApp *p, ucCmdOpt *cmd_opt) {
    char *command;
    const char *response;
    ucCmdTok *cmd_tok;
    ucCmd *cmd;
    ucCmdOpt *quit_opt, *help_opt;
    HelpState HelpState_s;
    QuitState QuitState_s;

    /* Create options for help and quit. */
    quit_opt = ucCmdOpt_create(quit, &QuitState_s, ucCmdApp_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    help_opt = ucCmdOpt_create(help, &HelpState_s, ucCmdApp_get_help_command(p), "Shows command information.", ucArgOpt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* Set the state used for the help and quit commands. */
    QuitState_s.app = p;
    HelpState_s.app = p;
    HelpState_s.cmd_opt = help_opt;

    /* Get this app's command object. */
    cmd = ucCmdApp_get_cmd(p);

    /* Show the banner. */
    ucCmd_respond(cmd, ucCmd_format_response(cmd, "Type %s to quit.", ucCmdApp_get_quit_command(p)));
    ucCmd_respond(cmd, ucCmd_format_response(cmd, "Type %s for help.", ucCmdApp_get_help_command(p)));

    /* Loop until quit. */
    for (;;) {

        /* Read the command. */
        command = ucCmdApp_receive(p);

        /* Parse the input into a command token. */
        cmd_tok = ucCmdParser_parse(ucCmdApp_get_cmd_parser(p), command);

        /* Set the command's parsed command token. */
        ucCmd_set_cmd_tok(cmd, cmd_tok);

        /* Process the command. */
        response = ucCmdOpt_process(help_opt, cmd);

        /* Check if we got a response. */
        if (response) {

            /* Check if the response is the escape response. */
            if (uc_STR_EQ(response, ucCmdApp_get_escape_response(p))) {
                
                /* We've been signaled to quit the app. */
                break;
            }
        }
    }

    /* Clear the options we created. */
    ucCmdOpt_destroy(quit_opt);
    ucCmdOpt_destroy(help_opt);
}
