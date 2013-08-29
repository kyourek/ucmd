#include <stdio.h>
#include <stdlib.h>
#include "cl_arg_tok.h"
#include "cl_cmd_parser.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_parser_tests.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_tests.h"
#include "cl_tok.h"

static cl_cmd_parser *get_cmd_parser(void) {
    return cl_cmd_parser_get_instance();
}

static cl_cmd_tok *parse_cmd(char *cmd) {
    return cl_cmd_parser_parse(get_cmd_parser(), cmd);
}

static char *cl_cmd_parser_parse_parses_command_value(void) {
    char cmd[20] = "command_name";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse returned null.", NULL != t);
    cl_assert("cl_cmd_parser_parse did not parse command value.", CL_TRUE == cl_tok_equals((cl_tok*)t, cmd));
    return 0;
}

static char *cl_cmd_parser_parse_parses_short_argument(void) {
    char cmd[24] = "command short_arg";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse did not parse a short argument.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "short_arg"));
    return 0;
}

static char *cl_cmd_parser_parse_parses_long_argument(void) {
    char cmd[31] = "command \"long argument\"";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse did not parse a long argument.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "long argument"));
    return 0;
}

static char *cl_cmd_parser_parse_parses_single_quotes(void) {
    char cmd[47] = "command 'long argument'";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse did not parse a long argument in single quotes.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "long argument"));
    return 0;
}

static char *cl_cmd_parser_parse_parses_switch(void) {
    char cmd[19] = "command -switch";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse did not parse switch.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-switch"));
    return 0;
}

static char *cl_cmd_parser_parse_parses_numeric_argument(void) {
    char cmd[38] = "command -12.34";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse added number as switch.", NULL == cl_cmd_tok_get_switch(t));
    cl_assert("cl_cmd_parser_parse did not parse numeric argument.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "-12.34"));
    return 0;
}

static char *cl_cmd_parser_parses_non_numeric_switch(void) {
    char cmd[23] = "command -32.4.0";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse parsed non-numeric switch as argument.", NULL == cl_cmd_tok_get_arg(t));
    cl_assert("cl_cmd_parser_parse failed to parse non-numeric switch.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-32.4.0"));
    return 0;
}

static char *cl_cmd_parser_parse_arguments_parsed_in_correct_order(void) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[81] = "command arg1 arg2 arg3 arg4 arg5 arg6 -s1 arg arg1 -s2 arg1 arg2 arg3";
    cl_arg_tok *arg;

    cl_cmd_tok *t = parse_cmd(cmd);

    i = 1;
    arg = cl_cmd_tok_get_arg(t); 
    while (arg) {
        sprintf(buf, "arg%d", i);
        cl_assert("cl_cmd_parser_parse did not parse arguments in correct order.", CL_TRUE == cl_tok_equals((cl_tok*)arg, buf));
        arg = cl_arg_tok_get_next(arg);
        if (arg) i++;
    }
    cl_assert("cl_cmd_parser_parse did not parse correct number of arguments.", 6 == i);
    return 0;
}

static char *cl_cmd_parser_parse_switches_parsed_in_correct_order(void) {
    int i = 0;
    char buf[10];
    char cmd[76] = "command arg1 arg2 -sw1 arg1 arg2 -sw2 -sw3 arg1 arg2 -sw4";
    cl_switch_tok *swtch;
    
    cl_cmd_tok *t = parse_cmd(cmd);

    swtch = cl_cmd_tok_get_switch(t);
    i = 1;
    while (swtch) {
        sprintf(buf, "-sw%d", i);
        cl_assert("cl_cmd_parser_parse did not parse switches in correct order.", CL_TRUE == cl_tok_equals((cl_tok*)swtch, buf));
        swtch = cl_switch_tok_get_next(swtch);
        if (swtch) i++;
    }
    cl_assert("cl_cmd_parser_parse did not parse correct number of switches.", 4 == i);
    return 0;
}

static char *cl_cmd_parser_parse_parses_switch_arguments_in_correct_order(void) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[94] = "command arg1 arg2 -sw1 arg1 arg2 arg3 arg4 arg5 -sw2 arg1 arg2";
    cl_arg_tok *arg;
    cl_switch_tok *swtch;

    cl_cmd_tok *t = parse_cmd(cmd);

    swtch = cl_cmd_tok_get_switch(t);
    arg = cl_switch_tok_get_arg(swtch);

    i = 1;
    while (arg) {
        sprintf(buf, "arg%d", i);
        cl_assert("cl_cmd_parser_parse did not parse switch arguments in correct order.", CL_TRUE == cl_tok_equals((cl_tok*)arg, buf));
        arg = cl_arg_tok_get_next(arg);
        if (arg) i++;
    }
    cl_assert("cl_cmd_parser_parse parsed incorrect number of switch arguments.", i == 5);
    return 0;
}

static char *cl_cmd_parser_parse_parses_trailing_quotes(void) {
    char cmd[38] = "cmd-name -s 'quoted \"arg\"'";
    cl_cmd_tok *t = parse_cmd(cmd);
    cl_assert("cl_cmd_parser_parse did not parse command value.", cl_tok_equals((cl_tok*)t, "cmd-name"));
    cl_assert("cl_cmd_parser_parse did not parse switch value.", cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-s"));
    cl_assert("cl_cmd_parser_parse did not parse quoted argument.", cl_tok_equals((cl_tok*)cl_switch_tok_get_arg(cl_cmd_tok_get_switch(t)), "quoted \"arg\""));
    return 0;
}

static char *cl_cmd_parser_parse_parses_command(void) {
    int i = 0;
    char cmd[139] = "somecmd 'this is a long argument' shortarg -73.452 -s1 sarg1 sarg2 sarg3 -s2 sarg3 'long switch argument' -12.43";
    cl_arg_tok *arg = NULL;
    cl_switch_tok *swtch = NULL;
    
    cl_cmd_tok *t = parse_cmd(cmd);

    cl_assert("cl_cmd_parser_parse parsing error occurred.", NULL != t);
    cl_assert("cl_cmd_parser_parse command value is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)t, "somecmd"));
    cl_assert("cl_cmd_parser_parse argument count is incorrect.", 3 == cl_arg_tok_count(cl_cmd_tok_get_arg(t)));
    cl_assert("cl_cmd_parser_parse switch count is incorrect.", 2 == cl_switch_tok_count(cl_cmd_tok_get_switch(t)));

    arg = cl_cmd_tok_get_arg(t);
    cl_assert("cl_cmd_parser_parse arg is null.", arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) cl_assert("cl_cmd_parser_parse arg 0 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "this is a long argument"));
        if (i == 1) cl_assert("cl_cmd_parser_parse arg 1 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "shortarg"));
        if (i == 2) cl_assert("cl_cmd_parser_parse arg 2 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "-73.452"));
        if (i == 3) cl_fail("cl_cmd_parser_parse incorrect arg loop.");
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    swtch = cl_cmd_tok_get_switch(t);
    cl_assert("cl_cmd_parser_parse switch is null.", swtch != NULL);
    i = 0; 
    while(swtch) {
        if (i == 0) cl_assert("cl_cmd_parser_parse switch 0 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s1"));
        if (i == 1) cl_assert("cl_cmd_parser_parse switch 1 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s2"));
        if (i == 2) cl_fail("cl_cmd_parser_parse incorrect switch loop.");
        i++; 
        swtch = cl_switch_tok_get_next(swtch);
    }

    swtch = cl_cmd_tok_get_switch(t);
    cl_assert("cl_cmd_parser_parse switch at index 0 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s1"));
    arg = cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)swtch);
    cl_assert("cl_cmd_parser_parse arg for switch 0 is null.", arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) cl_assert("cl_cmd_parser_parse switch 0 arg 0 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg1"));
        if (i == 1) cl_assert("cl_cmd_parser_parse switch 0 arg 1 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg2"));
        if (i == 2) cl_assert("cl_cmd_parser_parse switch 0 arg 2 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg3"));
        if (i == 3) cl_fail("cl_cmd_parser_parse incorrect switch 0 arg loop.");
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    swtch = cl_switch_tok_get_next(swtch);
    cl_assert("cl_cmd_parser_parse switch at index 1 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s2"));
    arg = cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)swtch);
    cl_assert("cl_cmd_parser_parse arg for switch 1 is null.", arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) cl_assert("cl_cmd_parser_parse switch 1 arg 0 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg3"));
        if (i == 1) cl_assert("cl_cmd_parser_parse switch 1 arg 1 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "long switch argument"));
        if (i == 2) cl_assert("cl_cmd_parser_parse switch 1 arg 2 is incorrect.", CL_TRUE == cl_tok_equals((cl_tok*)arg, "-12.43"));
        if (i == 3) cl_fail("cl_cmd_parser_parse incorrect switch 1 arg loop.");
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    return 0;
}

char *cl_cmd_parser_tests(void) {
    cl_run_test(cl_cmd_parser_parse_parses_command_value);
    cl_run_test(cl_cmd_parser_parse_parses_short_argument);
    cl_run_test(cl_cmd_parser_parse_parses_long_argument);
    cl_run_test(cl_cmd_parser_parse_parses_single_quotes);
    cl_run_test(cl_cmd_parser_parse_parses_switch);
    cl_run_test(cl_cmd_parser_parse_parses_numeric_argument);
    cl_run_test(cl_cmd_parser_parses_non_numeric_switch);
    cl_run_test(cl_cmd_parser_parse_arguments_parsed_in_correct_order);
    cl_run_test(cl_cmd_parser_parse_switches_parsed_in_correct_order);
    cl_run_test(cl_cmd_parser_parse_parses_switch_arguments_in_correct_order);
    cl_run_test(cl_cmd_parser_parse_parses_command);
    cl_run_test(cl_cmd_parser_parse_parses_trailing_quotes);
    return 0;
}
