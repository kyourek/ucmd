#include "ucmd_internal.h"

ucInstance_INIT(ucApp, ucApp_COUNT);

typedef struct {
    ucApp *app;
    ucCmdOpt *cmd_opt;
} HelpState;

typedef struct {
    ucApp *app;
} QuitState;

static const char *quit(ucCmd *cmd, void *state) {
    QuitState *s = (QuitState*)state;
    assert(s);
    return ucApp_get_escape_response(s->app);
}

static const char *help(ucCmd *cmd, void *state) {
    HelpState *s = (HelpState*)state;
    ucArgTok *arg_tok;
    ucCmdOpt *cmd_opt;

    assert(s);

    cmd_opt = s->cmd_opt;
    arg_tok = ucCmdTok_get_arg(ucCmd_get_command(cmd));
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
        ucApp_get_help_command(s->app));
}

void ucApp_set_escape_response(ucApp *p, const char *value) {
    assert(p);
    p->escape_response = value;
}

const char *ucApp_get_escape_response(ucApp *p) {
    assert(p);
    return p->escape_response;
}

void ucApp_set_quit_command(ucApp *p, const char *value) {
    assert(p);
    p->quit_command = value;
}

const char *ucApp_get_quit_command(ucApp *p) {
    assert(p);
    return p->quit_command;
}

void ucApp_set_help_command(ucApp *p, const char *value) {
    assert(p);
    p->help_command = value;
}

const char *ucApp_get_help_command(ucApp *p) {
    assert(p);
    return p->help_command;
}

ucCmd *ucApp_get_cmd(ucApp *p) {
    assert(p);
    return p->cmd;
}

ucApp *ucApp_init(ucApp *p, ucCmd *cmd) {
    assert(p);
    p->cmd = cmd;
    p->help_command = "help";
    p->quit_command = "quit";
    p->escape_response = "\x1b";
    return p;
}

ucApp *ucApp_create(void) {
    return ucApp_init(
        ucInstance_create(),
        ucCmd_create());
}

void ucApp_destroy(ucApp *p) {
    if (p) {
        if (p->cmd) {
            ucCmd_destroy(p->cmd);
        }
        ucInstance_destroy(p);
    }
}

void ucApp_run(ucApp *p, ucCmdOpt *cmd_opt) {
    const char *response;
    ucCmd *cmd;
    ucCmdOpt *quit_opt, *help_opt;
    HelpState HelpState_s;
    QuitState QuitState_s;

    /* Create options for help and quit. */
    quit_opt = ucCmdOpt_create(quit, &QuitState_s, ucApp_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    help_opt = ucCmdOpt_create(help, &HelpState_s, ucApp_get_help_command(p), "Shows command information.", ucArgOpt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* Set the state used for the help and quit commands. */
    QuitState_s.app = p;
    HelpState_s.app = p;
    HelpState_s.cmd_opt = help_opt;

    /* Get this app's command object. */
    cmd = ucApp_get_cmd(p);

    /* Show the banner. */
    ucCmd_respond(cmd, ucCmd_format_response(cmd, "Type %s to quit.", ucApp_get_quit_command(p)));
    ucCmd_respond(cmd, ucCmd_format_response(cmd, "Type %s for help.", ucApp_get_help_command(p)));

    /* Loop until quit. */
    for (;;) {

        /* Read the command. */
        ucCmd_listen(cmd);

        /* Process the command. */
        response = ucCmdOpt_process(help_opt, cmd);

        /* Check if we got a response. */
        if (response) {

            /* Check if the response is the escape response. */
            if (uc_STR_EQ(response, ucApp_get_escape_response(p))) {
                
                /* We've been signaled to quit the app. */
                break;
            }
        }
    }

    /* Clear the options we created. */
    ucCmdOpt_destroy(quit_opt);
    ucCmdOpt_destroy(help_opt);
}
