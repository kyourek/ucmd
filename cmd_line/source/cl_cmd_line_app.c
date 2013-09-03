#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cl_arg_opt.h"
#include "cl_arg_tok.h"
#include "cl_cmd_parser.h"
#include "cl_cmd_parser_p.h"
#include "cl_opt.h"
#include "cl_switch_opt.h"
#include "cl_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_app.h"
#include "cl_cmd_line_app_p.h"
#include "cl_cmd_line_opt.h"

typedef struct help_state {
    cl_cmd_line_app *app;
    cl_cmd_line_opt *cmd_opt;
} help_state;

typedef struct quit_state {
    cl_cmd_line_app *app;
} quit_state;

static char *receive(char *buf, int buf_size) {
#ifdef CL_CMD_LINE_APP_RECEIVE_FGETS_STDIN
    return fgets(buf, buf_size, stdin);
#else
    return NULL;
#endif
}

static char *cl_cmd_line_app_receive(cl_cmd_line_app *p) {
    if (NULL == p) return 0;
    if (NULL == p->receive) return 0;
    return p->receive(p->cmd_buf, sizeof(p->cmd_buf) / sizeof(char));
}

static const char *quit(cl_cmd_line *cmd, void *state) {
    quit_state *s = (quit_state*)state;
    return cl_cmd_line_app_get_escape_response(s->app);
}

static void show_opt_help(cl_opt *opt, cl_cmd_line *cmd, const char *prefix) {
    const char *format = cl_opt_is_required(opt)
        ? "%s%s: %s"
        : "%s[%s]: %s";
    cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, format, prefix, cl_opt_get_name(opt), cl_opt_get_desc(opt)));
}

static void show_cmd_opt_usage(cl_cmd_line_opt *cmd_opt, cl_cmd_line *cmd) {
    cl_arg_opt *arg_opt;
    cl_switch_opt *switch_opt;
    const char *format;
    const char *response;

    response = cl_cmd_line_format_response(cmd, "%s", cl_opt_get_name((cl_opt*)cmd_opt));

    arg_opt = cl_cmd_line_opt_get_arg_opt(cmd_opt);
    while (NULL != arg_opt) {
        
        format = cl_opt_is_required((cl_opt*)arg_opt) ? "%s %s" : "%s [%s]";
        response = cl_cmd_line_format_response(cmd, format, response, cl_opt_get_name((cl_opt*)arg_opt));

        arg_opt = cl_arg_opt_get_next(arg_opt);
    }

    switch_opt = cl_cmd_line_opt_get_switch_opt(cmd_opt);
    while (NULL != switch_opt) {
        
        format = cl_opt_is_required((cl_opt*)switch_opt) ? "%s %s" : "%s [%s]";
        response = cl_cmd_line_format_response(cmd, format, response, cl_opt_get_name((cl_opt*)switch_opt));

        arg_opt = cl_switch_opt_get_arg_opt(switch_opt);
        while (NULL != arg_opt) {

            format = cl_opt_is_required((cl_opt*)arg_opt) ? "%s %s" : "%s [%s]";
            response = cl_cmd_line_format_response(cmd, format, response, cl_opt_get_name((cl_opt*)arg_opt));

            arg_opt = cl_arg_opt_get_next(arg_opt);
        }

        switch_opt = cl_switch_opt_get_next(switch_opt);
    }

    cl_cmd_line_respond(cmd, response);
}

static void show_cmd_opt_help(cl_cmd_line_opt *cmd_opt, cl_cmd_line *cmd) {
    cl_arg_opt *arg_opt;
    cl_switch_opt *switch_opt;

    show_cmd_opt_usage(cmd_opt, cmd);

    show_opt_help((cl_opt*)cmd_opt, cmd, "");
        
    arg_opt = cl_cmd_line_opt_get_arg_opt(cmd_opt);
    while (NULL != arg_opt) {
        show_opt_help((cl_opt*)arg_opt, cmd, "\t");
        arg_opt = cl_arg_opt_get_next(arg_opt);
    }

    switch_opt = cl_cmd_line_opt_get_switch_opt(cmd_opt);
    while (NULL != switch_opt) {
        show_opt_help((cl_opt*)switch_opt, cmd, "\t");

        arg_opt = cl_switch_opt_get_arg_opt(switch_opt);            
        while (NULL != arg_opt) {
            show_opt_help((cl_opt*)arg_opt, cmd, "\t\t");
            arg_opt = cl_arg_opt_get_next(arg_opt);
        }
        switch_opt = cl_switch_opt_get_next(switch_opt);
    }
}

static const char *help(cl_cmd_line *cmd, void *state) {
    help_state *s;
    cl_arg_tok *arg_tok;
    cl_cmd_line_opt *cmd_opt;

    s = (help_state*)state;
    cmd_opt = s->cmd_opt;

    arg_tok = cl_cmd_tok_get_arg(cl_cmd_line_get_cmd_tok(cmd));
    if (NULL != arg_tok) {
        cmd_opt = cl_cmd_line_opt_find_by_name(cmd_opt, cl_tok_get_value(arg_tok));
        if (NULL != cmd_opt) {
            show_cmd_opt_help(cmd_opt, cmd);
            return 0;
        }
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Invalid command: no option found for \"%s\".", cl_tok_get_value(arg_tok)));
        return 0;
    }

    cl_cmd_line_respond(cmd, "Commands");
    while (NULL != cmd_opt) {
        show_opt_help((cl_opt*)cmd_opt, cmd, "\t");
        cmd_opt = cl_cmd_line_opt_get_next(cmd_opt);
    }
    cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Type \"%s\" followed by a command name for additional help with that command.", cl_cmd_line_app_get_help_command(s->app)));

    return 0;
}

static CL_ERR run(cl_cmd_line_app *p, cl_cmd_line_opt *cmd_opt) {
    char *command;
    const char *response;
    cl_cmd_tok *cmd_tok;
    cl_cmd_line *cmd;
    cl_cmd_line_opt *quit_opt, *main_opt;
    help_state help_state_s;
    quit_state quit_state_s;

    if (NULL == p) return -1;

    /* create options for help and quit */
    quit_opt = cl_cmd_line_opt_create(quit, &quit_state_s, cl_cmd_line_app_get_quit_command(p), "Exits the command interface.", NULL, NULL, cmd_opt);
    main_opt = cl_cmd_line_opt_create(help, &help_state_s, cl_cmd_line_app_get_help_command(p), "Shows command information.", cl_arg_opt_create("<command>", "If provided, help is shown for the given command.", NULL), NULL, quit_opt);

    /* set the state used for the help and quit commands */
    quit_state_s.app = p;
    help_state_s.app = p;
    help_state_s.cmd_opt = main_opt;

    /* get this app's command object */
    cmd = cl_cmd_line_app_get_cmd(p);

    /* show the banner */
    cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Type %s to quit.", cl_cmd_line_app_get_quit_command(p)));
    cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Type %s for help.", cl_cmd_line_app_get_help_command(p)));

    /* loop until we quit */
    for (;;) {
                
        /* read the command */
        command = cl_cmd_line_app_receive(p);

        /* parse the input into a command token */
        cmd_tok = cl_cmd_parser_parse(cl_cmd_line_app_get_cmd_parser(p), command);

        /* set the command's parsed command token */
        cl_cmd_line_set_cmd_tok(cmd, cmd_tok);

        /* process the command */
        response = cl_cmd_line_opt_process(main_opt, cmd);

        /* check if we got a response */
        if (NULL != response) {

            /* check if the response is the escape response */
            if (0 == strcmp(response, cl_cmd_line_app_get_escape_response(p))) {

                /* we've been signaled to quit the app */
                break;
            }

            /* send the response */
            cl_cmd_line_respond(cmd, response);
        }
    }

    /* clear the options we created */
    cl_cmd_line_opt_destroy(quit_opt);
    cl_cmd_line_opt_destroy(main_opt);

    /* if we got here, then the app was quit */
    return 0;
}

void cl_cmd_line_app_set_escape_response(cl_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->escape_response = value;
}

const char *cl_cmd_line_app_get_escape_response(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->escape_response;
}

void cl_cmd_line_app_set_receive(cl_cmd_line_app *p, cl_cmd_line_app_receive_func *value) {
    if (NULL == p) return;
    p->receive = value;
}

cl_cmd_line_app_receive_func *cl_cmd_line_app_get_receive(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->receive;
}

void cl_cmd_line_app_set_quit_command(cl_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->quit_command = value;
}

const char *cl_cmd_line_app_get_quit_command(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->quit_command;
}

void cl_cmd_line_app_set_help_command(cl_cmd_line_app *p, const char *value) {
    if (NULL == p) return;
    p->help_command = value;
}

const char *cl_cmd_line_app_get_help_command(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->help_command;
}

void cl_cmd_line_app_set_cmd(cl_cmd_line_app *p, cl_cmd_line *value) {
    if (NULL == p) return;
    p->cmd = value;
}

cl_cmd_line *cl_cmd_line_app_get_cmd(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->cmd;
}

cl_cmd_parser *cl_cmd_line_app_get_cmd_parser(cl_cmd_line_app *p) {
    if (NULL == p) return NULL;
    return p->cmd_parser;
}

cl_cmd_line_app *cl_cmd_line_app_get_instance(void) {
    static cl_cmd_line_app instance = { 0 };
    static cl_cmd_line_app *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->cmd = cl_cmd_line_get_instance();
        p->cmd_parser = cl_cmd_parser_get_instance();
        p->run = run;
        p->receive = receive;
        p->help_command = "help";
        p->quit_command = "quit";
        p->escape_response = "\x1b";
    }
    return p;
}

CL_ERR cl_cmd_line_app_run(cl_cmd_line_app *p, cl_cmd_line_opt *cmd_opt) {
    if (NULL == p) return -1;
    if (NULL == p->run) return -2;
    return p->run(p, cmd_opt);
}
