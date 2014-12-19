#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ucArgOpt.h"
#include "uc_arg_tok.h"
#include "uc_cmd_line.h"
#include "uc_cmd_line_app_p.h"
#include "uc_cmd_line_opt.h"
#include "uc_cmd_parser_p.h"
#include "uc_switch_opt.h"

typedef struct help_state {
    uc_cmd_line_app *app;
    uc_cmd_line_opt *cmd_opt;
} help_state;

typedef struct quit_state {
    uc_cmd_line_app *app;
} quit_state;

static const char *quit(uc_cmd_line *cmd, void *state) {
    quit_state *s = (quit_state*)state;
    return uc_cmd_line_app_get_escape_response(s->app);
}

static const char *help(uc_cmd_line *cmd, void *state) {
    help_state *s;
    uc_arg_tok *arg_tok;
    uc_cmd_line_opt *cmd_opt;

    s = (help_state*)state;
    cmd_opt = s->cmd_opt;

    arg_tok = uc_cmd_tok_get_arg(uc_cmd_line_get_cmd_tok(cmd));
    if (NULL != arg_tok) {
        cmd_opt = uc_cmd_line_opt_find_by_name(cmd_opt, uc_tok_get_value(arg_tok));
        if (NULL != cmd_opt) {
            uc_cmd_line_opt_send_help(cmd_opt, cmd);
            return 0;
        }
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Invalid command: no option found for \"%s\".", uc_tok_get_value(arg_tok)));
        return 0;
    }

    uc_cmd_line_respond(cmd, "Commands");
    while (NULL != cmd_opt) {
        uc_opt_send_help((uc_opt*)cmd_opt, cmd, "\t");
        cmd_opt = uc_cmd_line_opt_get_next(cmd_opt);
    }
    uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Type \"%s\" followed by a command name for additional help with that command.", uc_cmd_line_app_get_help_command(s->app)));

    return 0;
}

static uc_err run(uc_cmd_line_app *p, uc_cmd_line_opt *cmd_opt) {
    char *command;
    const char *response;
    uc_cmd_tok *cmd_tok;
    uc_cmd_line *cmd;
    uc_cmd_line_opt *quit_opt, *main_opt;
    help_state help_state_s;
    quit_state quit_state_s;

    if (NULL == p) return -1;

    /* create options for help and quit */
    quit_opt = uc_cmd_line_opt_create(quit, &quit_state_s, uc_cmd_line_app_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    main_opt = uc_cmd_line_opt_create(help, &help_state_s, uc_cmd_line_app_get_help_command(p), "Shows command information.", ucArgOpt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* set the state used for the help and quit commands */
    quit_state_s.app = p;
    help_state_s.app = p;
    help_state_s.cmd_opt = main_opt;

    /* get this app's command object */
    cmd = uc_cmd_line_app_get_cmd(p);

    /* show the banner */
    uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Type %s to quit.", uc_cmd_line_app_get_quit_command(p)));
    uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Type %s for help.", uc_cmd_line_app_get_help_command(p)));

    /* loop until we quit */
    for (;;) {
                
        /* read the command */
        command = uc_cmd_line_app_receive(p);

        /* parse the input into a command token */
        cmd_tok = uc_cmd_parser_parse(uc_cmd_line_app_get_cmd_parser(p), command);

        /* set the command's parsed command token */
        uc_cmd_line_set_cmd_tok(cmd, cmd_tok);

        /* process the command */
        response = uc_cmd_line_opt_process(main_opt, cmd);

        /* check if we got a response */
        if (NULL != response) {

            /* check if the response is the escape response */
            if (0 == strcmp(response, uc_cmd_line_app_get_escape_response(p))) {

                /* we've been signaled to quit the app */
                break;
            }

            /* send the response */
            uc_cmd_line_respond(cmd, response);
        }
    }

    /* clear the options we created */
    uc_cmd_line_opt_destroy(quit_opt);
    uc_cmd_line_opt_destroy(main_opt);

    /* if we got here, then the app was quit */
    return UC_ERR_NONE;
}

void uc_cmd_line_app_set_escape_response(uc_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->escape_response = value;
}

const char *uc_cmd_line_app_get_escape_response(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->escape_response;
}

void uc_cmd_line_app_set_receive(uc_cmd_line_app *p, uc_cmd_line_app_receive_func *value) {
    if (NULL == p) return;
    p->receive = value;
}

uc_cmd_line_app_receive_func *uc_cmd_line_app_get_receive(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->receive;
}

void *uc_cmd_line_app_get_receive_state(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->receive_state;
}

void uc_cmd_line_app_set_receive_state(uc_cmd_line_app *p, void *value) {
    if (NULL == p) return;
    p->receive_state = value;
}

void uc_cmd_line_app_set_quit_command(uc_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->quit_command = value;
}

const char *uc_cmd_line_app_get_quit_command(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->quit_command;
}

void uc_cmd_line_app_set_help_command(uc_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->help_command = value;
}

const char *uc_cmd_line_app_get_help_command(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->help_command;
}

void uc_cmd_line_app_set_cmd(uc_cmd_line_app *p, uc_cmd_line *value) {
    if (NULL == p) return;
    p->cmd = value;
}

uc_cmd_line *uc_cmd_line_app_get_cmd(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->cmd;
}

uc_cmd_parser *uc_cmd_line_app_get_cmd_parser(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->cmd_parser;
}

uc_cmd_line_app *uc_cmd_line_app_get_instance(void) {
    static uc_cmd_line_app instance = { 0 };
    static uc_cmd_line_app *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->cmd = uc_cmd_line_get_instance();
        p->cmd_parser = uc_cmd_parser_get_instance();
        p->run = run;
        p->receive = NULL;
        p->receive_state = NULL;
        p->help_command = "help";
        p->quit_command = "quit";
        p->escape_response = "\x1b";
    }
    return p;
}

uc_err uc_cmd_line_app_run(uc_cmd_line_app *p, uc_cmd_line_opt *cmd_opt) {
    if (NULL == p) return -1;
    if (NULL == p->run) return -2;
    return p->run(p, cmd_opt);
}

char *uc_cmd_line_app_receive(uc_cmd_line_app *p) {
    if (NULL == p) return NULL;
    if (NULL == p->receive) return NULL;
    return p->receive(p->cmd_buf, sizeof(p->cmd_buf), p->receive_state);
}
