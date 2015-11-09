#include "ucmd_internal.h"

ucInstance_INIT(ucCmdLineApp, ucCmdLineApp_COUNT);

typedef struct {
    ucCmdLineApp *app;
    ucCmdLineOpt *cmd_opt;
} HelpState;

typedef struct {
    ucCmdLineApp *app;
} QuitState;

static const char *quit(ucCmdLine *cmd, void *state) {
    QuitState *s = (QuitState*)state;
    assert(s);
    return ucCmdLineApp_get_escape_response(s->app);
}

static const char *help(ucCmdLine *cmd, void *state) {
    HelpState *s = (HelpState*)state;
    ucArgTok *arg_tok;
    ucCmdLineOpt *cmd_opt;

    assert(s);

    cmd_opt = s->cmd_opt;
    arg_tok = ucCmdTok_get_arg(ucCmdLine_get_cmd_tok(cmd));
    if (arg_tok) {
        cmd_opt = ucCmdLineOpt_find_by_name(cmd_opt, arg_tok);
        if (cmd_opt) {
            ucCmdLineOpt_send_help(cmd_opt, cmd);
            return NULL;
        }
        return ucCmdLine_format_response(
            cmd, 
            ucOpt_INVALID "No command option found for \"%s\".", 
            arg_tok);
    }

    ucCmdLine_respond(cmd, "Commands");

    while (cmd_opt) {
        ucOpt_send_help((ucOpt*)cmd_opt, cmd, "\t");
        cmd_opt = ucCmdLineOpt_get_next(cmd_opt);
    }

    return ucCmdLine_format_response(
        cmd, 
        "Type \"%s\" followed by a command name for additional help with that command.", 
        ucCmdLineApp_get_help_command(s->app));
}

void ucCmdLineApp_set_escape_response(ucCmdLineApp *p, const char *value) {
    assert(p);
    p->escape_response = value;
}

const char *ucCmdLineApp_get_escape_response(ucCmdLineApp *p) {
    assert(p);
    return p->escape_response;
}

void ucCmdLineApp_set_receive(ucCmdLineApp *p, ucCmdLineApp_ReceiveFunc *value) {
    assert(p);
    p->receive = value;
}

ucCmdLineApp_ReceiveFunc *ucCmdLineApp_get_receive(ucCmdLineApp *p) {
    assert(p);
    return p->receive;
}

void *ucCmdLineApp_get_receive_state(ucCmdLineApp *p) {
    assert(p);
    return p->receive_state;
}

void ucCmdLineApp_set_receive_state(ucCmdLineApp *p, void *value) {
    assert(p);
    p->receive_state = value;
}

void ucCmdLineApp_set_quit_command(ucCmdLineApp *p, const char *value) {
    assert(p);
    p->quit_command = value;
}

const char *ucCmdLineApp_get_quit_command(ucCmdLineApp *p) {
    assert(p);
    return p->quit_command;
}

void ucCmdLineApp_set_help_command(ucCmdLineApp *p, const char *value) {
    assert(p);
    p->help_command = value;
}

const char *ucCmdLineApp_get_help_command(ucCmdLineApp *p) {
    assert(p);
    return p->help_command;
}

ucCmdLine *ucCmdLineApp_get_cmd(ucCmdLineApp *p) {
    assert(p);
    return p->cmd;
}

ucCmdParser *ucCmdLineApp_get_cmd_parser(ucCmdLineApp *p) {
    assert(p);
    return p->cmd_parser;
}

ucCmdLineApp *ucCmdLineApp_init(ucCmdLineApp *p, ucCmdParser *cmd_parser, ucCmdLine *cmd) {
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

ucCmdLineApp *ucCmdLineApp_create(void) {
    return ucCmdLineApp_init(
        ucInstance_create(),
        ucCmdParser_create(),
        ucCmdLine_create());
}

void ucCmdLineApp_destroy(ucCmdLineApp *p) {
    if (p) {
        ucCmdLine_destroy(p->cmd);
        ucCmdParser_destroy(p->cmd_parser);
    }
    ucInstance_destroy(p);
}

char *ucCmdLineApp_receive(ucCmdLineApp *p) {
    assert(p);
    if (p->receive) {
        return p->receive(p->cmd_str, sizeof(p->cmd_str), p->receive_state);
    }
    return NULL;
}

size_t ucCmdLineApp_get_cmd_str_size_max(ucCmdLineApp *p) {
    assert(p);
    return (sizeof(p->cmd_str) / sizeof(p->cmd_str[0])) - 1;
}

void ucCmdLineApp_run(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt) {
    char *command;
    const char *response;
    ucCmdTok *cmd_tok;
    ucCmdLine *cmd;
    ucCmdLineOpt *quit_opt, *help_opt;
    HelpState HelpState_s;
    QuitState QuitState_s;

    /* Create options for help and quit. */
    quit_opt = ucCmdLineOpt_create(quit, &QuitState_s, ucCmdLineApp_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    help_opt = ucCmdLineOpt_create(help, &HelpState_s, ucCmdLineApp_get_help_command(p), "Shows command information.", ucArgOpt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* Set the state used for the help and quit commands. */
    QuitState_s.app = p;
    HelpState_s.app = p;
    HelpState_s.cmd_opt = help_opt;

    /* Get this app's command object. */
    cmd = ucCmdLineApp_get_cmd(p);

    /* Show the banner. */
    ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Type %s to quit.", ucCmdLineApp_get_quit_command(p)));
    ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Type %s for help.", ucCmdLineApp_get_help_command(p)));

    /* Loop until quit. */
    for (;;) {

        /* Read the command. */
        command = ucCmdLineApp_receive(p);

        /* Parse the input into a command token. */
        cmd_tok = ucCmdParser_parse(ucCmdLineApp_get_cmd_parser(p), command);

        /* Set the command's parsed command token. */
        ucCmdLine_set_cmd_tok(cmd, cmd_tok);

        /* Process the command. */
        response = ucCmdLineOpt_process(help_opt, cmd);

        /* Check if we got a response. */
        if (response) {

            /* Check if the response is the escape response. */
            if (uc_STR_EQ(response, ucCmdLineApp_get_escape_response(p))) {
                
                /* We've been signaled to quit the app. */
                break;
            }
        }
    }

    /* Clear the options we created. */
    ucCmdLineOpt_destroy(quit_opt);
    ucCmdLineOpt_destroy(help_opt);
}
