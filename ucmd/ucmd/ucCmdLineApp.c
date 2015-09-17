#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ucmd_internal.h"

typedef struct HelpState {
    ucCmdLineApp *app;
    ucCmdLineOpt *cmd_opt;
} HelpState;

typedef struct QuitState {
    ucCmdLineApp *app;
} QuitState;

static const char *quit(ucCmdLine *cmd, void *state) {
    QuitState *s = (QuitState*)state;
    return ucCmdLineApp_get_escape_response(s->app);
}

static const char *help(ucCmdLine *cmd, void *state) {
    HelpState *s;
    ucArgTok *arg_tok;
    ucCmdLineOpt *cmd_opt;

    s = (HelpState*)state;
    cmd_opt = s->cmd_opt;

    arg_tok = ucCmdTok_get_arg(ucCmdLine_get_cmd_tok(cmd));
    if (NULL != arg_tok) {
        cmd_opt = ucCmdLineOpt_find_by_name(cmd_opt, ucTok_get_value(arg_tok));
        if (NULL != cmd_opt) {
            ucCmdLineOpt_send_help(cmd_opt, cmd);
            return 0;
        }
        ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Invalid command: no option found for \"%s\".", ucTok_get_value(arg_tok)));
        return 0;
    }

    ucCmdLine_respond(cmd, "Commands");
    while (NULL != cmd_opt) {
        ucOpt_send_help((ucOpt*)cmd_opt, cmd, "\t");
        cmd_opt = ucCmdLineOpt_get_next(cmd_opt);
    }
    ucCmdLine_respond(cmd, ucCmdLine_format_response(cmd, "Type \"%s\" followed by a command name for additional help with that command.", ucCmdLineApp_get_help_command(s->app)));

    return 0;
}

static void terminate_response(ucCmdLineApp *p) {
    ucCmdLine *cmd;
    const char *response_terminator = ucCmdLineApp_get_response_terminator(p);
    if (response_terminator) {
        cmd = ucCmdLineApp_get_cmd(p);
        ucCmdLine_respond(cmd, response_terminator);
    }
}

static ucErr run(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt) {
    char *command;
    const char *response;
    ucCmdTok *cmd_tok;
    ucCmdLine *cmd;
    ucCmdLineOpt *quit_opt, *main_opt;
    HelpState HelpState_s;
    QuitState QuitState_s;

    if (NULL == p) return -1;

    /* Create options for help and quit. */
    quit_opt = ucCmdLineOpt_create(quit, &QuitState_s, ucCmdLineApp_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    main_opt = ucCmdLineOpt_create(help, &HelpState_s, ucCmdLineApp_get_help_command(p), "Shows command information.", ucArgOpt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* Set the state used for the help and quit commands. */
    QuitState_s.app = p;
    HelpState_s.app = p;
    HelpState_s.cmd_opt = main_opt;

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
        response = ucCmdLineOpt_process(main_opt, cmd);

        /* Check if we got a response. */
        if (response) {

            /* Check if the response is the escape response. */
            if (0 == strcmp(response, ucCmdLineApp_get_escape_response(p))) {

                /* We've been signaled to quit the app. */
                break;
            }

            /* Send the response. */
            ucCmdLine_respond(cmd, response);
        }

        /* Signal that this command's response is complete. */
        terminate_response(p);
    }

    /* Clear the options we created. */
    ucCmdLineOpt_destroy(quit_opt);
    ucCmdLineOpt_destroy(main_opt);

    /* If we got here, then the app was quit. */
    return ucErr_NONE;
}

void ucCmdLineApp_set_escape_response(ucCmdLineApp *p, const char *value) {
    if (NULL == p) return;
    p->escape_response = value;
}

const char *ucCmdLineApp_get_escape_response(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->escape_response;
}

void ucCmdLineApp_set_receive(ucCmdLineApp *p, ucCmdLineApp_ReceiveFunc *value) {
    if (NULL == p) return;
    p->receive = value;
}

ucCmdLineApp_ReceiveFunc *ucCmdLineApp_get_receive(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->receive;
}

void *ucCmdLineApp_get_receive_state(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->receive_state;
}

void ucCmdLineApp_set_receive_state(ucCmdLineApp *p, void *value) {
    if (NULL == p) return;
    p->receive_state = value;
}

void ucCmdLineApp_set_quit_command(ucCmdLineApp *p, const char *value) {
    if (NULL == p) return;
    p->quit_command = value;
}

const char *ucCmdLineApp_get_quit_command(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->quit_command;
}

void ucCmdLineApp_set_help_command(ucCmdLineApp *p, const char *value) {
    if (NULL == p) return;
    p->help_command = value;
}

const char *ucCmdLineApp_get_help_command(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->help_command;
}

void ucCmdLineApp_set_cmd(ucCmdLineApp *p, ucCmdLine *value) {
    if (NULL == p) return;
    p->cmd = value;
}

ucCmdLine *ucCmdLineApp_get_cmd(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->cmd;
}

ucCmdParser *ucCmdLineApp_get_cmd_parser(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->cmd_parser;
}

void ucCmdLineApp_set_response_terminator(ucCmdLineApp *p, const char *value) {
    if (NULL == p) return;
    p->response_terminator = value;
}

const char *ucCmdLineApp_get_response_terminator(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    return p->response_terminator;
}

ucCmdLineApp *ucCmdLineApp_init(ucCmdLineApp *p, ucCmdParser *cmd_parser, ucCmdLine *cmd) {
    if (NULL == p) return NULL;
    p->cmd = cmd;
    p->cmd_parser = cmd_parser;
    p->run = run;
    p->receive = NULL;
    p->receive_state = NULL;
    p->help_command = "help";
    p->quit_command = "quit";
    p->escape_response = "\x1b";
    p->response_terminator = NULL;
    return p;
}

ucCmdLineApp *ucCmdLineApp_get_instance(void) {
    static ucCmdLineApp instance = { 0 };
    static ucCmdLineApp *pointer = NULL;
    if (pointer == NULL) {
        pointer = ucCmdLineApp_init(&instance, ucCmdParser_get_instance(), ucCmdLine_get_instance());
    }
    return pointer;
}

ucErr ucCmdLineApp_run(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt) {
    if (NULL == p) return -1;
    if (NULL == p->run) return -2;
    return p->run(p, cmd_opt);
}

char *ucCmdLineApp_receive(ucCmdLineApp *p) {
    if (NULL == p) return NULL;
    if (NULL == p->receive) return NULL;
    return p->receive(p->cmd_buf, sizeof(p->cmd_buf), p->receive_state);
}
