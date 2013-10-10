#include <stdio.h>
#include <stdlib.h>
#include "cl_arg_tok.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_parser_tests.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_cmd_parser *get_cmd_parser(void) {
    return cl_cmd_parser_get_instance();
}

static cl_cmd_tok *parse_cmd(char *cmd) {
    return cl_cmd_parser_parse(get_cmd_parser(), cmd);
}

static cl_test_err cl_cmd_parser_parse_parses_command_value(cl_test_group *p) {
    char cmd[20] = "command_name";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(NULL != t);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)t, cmd));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_short_argument(cl_test_group *p) {
    char cmd[24] = "command short_arg";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "short_arg"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_long_argument(cl_test_group *p) {
    char cmd[31] = "command \"long argument\"";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "long argument"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_single_quotes(cl_test_group *p) {
    char cmd[47] = "command 'long argument'";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "long argument"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_switch(cl_test_group *p) {
    char cmd[19] = "command -switch";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-switch"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_numeric_argument(cl_test_group *p) {
    char cmd[38] = "command -12.34";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(NULL == cl_cmd_tok_get_switch(t));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(t), "-12.34"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parses_non_numeric_switch(cl_test_group *p) {
    char cmd[23] = "command -32.4.0";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(NULL == cl_cmd_tok_get_arg(t));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-32.4.0"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_arguments_parsed_in_correct_order(cl_test_group *p) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[81] = "command arg1 arg2 arg3 arg4 arg5 arg6 -s1 arg arg1 -s2 arg1 arg2 arg3";
    cl_arg_tok *arg;

    cl_cmd_tok *t = parse_cmd(cmd);

    i = 1;
    arg = cl_cmd_tok_get_arg(t); 
    while (arg) {
        sprintf(buf, "arg%d", i);
        CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, buf));
        arg = cl_arg_tok_get_next(arg);
        if (arg) i++;
    }
    CL_TEST_ASSERT(6 == i);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_switches_parsed_in_correct_order(cl_test_group *p) {
    int i = 0;
    char buf[10];
    char cmd[76] = "command arg1 arg2 -sw1 arg1 arg2 -sw2 -sw3 arg1 arg2 -sw4";
    cl_switch_tok *swtch;
    
    cl_cmd_tok *t = parse_cmd(cmd);

    swtch = cl_cmd_tok_get_switch(t);
    i = 1;
    while (swtch) {
        sprintf(buf, "-sw%d", i);
        CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)swtch, buf));
        swtch = cl_switch_tok_get_next(swtch);
        if (swtch) i++;
    }
    CL_TEST_ASSERT(4 == i);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_switch_arguments_in_correct_order(cl_test_group *p) {
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
        CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, buf));
        arg = cl_arg_tok_get_next(arg);
        if (arg) i++;
    }
    CL_TEST_ASSERT(i == 5);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_trailing_quotes(cl_test_group *p) {
    char cmd[38] = "cmd-name -s 'quoted \"arg\"'";
    cl_cmd_tok *t = parse_cmd(cmd);
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)t, "cmd-name"));
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(t), "-s"));
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)cl_switch_tok_get_arg(cl_cmd_tok_get_switch(t)), "quoted \"arg\""));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_parser_parse_parses_command(cl_test_group *p) {
    int i = 0;
    char cmd[139] = "somecmd 'this is a long argument' shortarg -73.452 -s1 sarg1 sarg2 sarg3 -s2 sarg3 'long switch argument' -12.43";
    cl_arg_tok *arg = NULL;
    cl_switch_tok *swtch = NULL;
    
    cl_cmd_tok *t = parse_cmd(cmd);

    CL_TEST_ASSERT(NULL != t);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)t, "somecmd"));
    CL_TEST_ASSERT(3 == cl_arg_tok_count(cl_cmd_tok_get_arg(t)));
    CL_TEST_ASSERT(2 == cl_switch_tok_count(cl_cmd_tok_get_switch(t)));

    arg = cl_cmd_tok_get_arg(t);
    CL_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "this is a long argument"));
        if (i == 1) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "shortarg"));
        if (i == 2) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "-73.452"));
        if (i == 3) CL_TEST_FAIL();
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    swtch = cl_cmd_tok_get_switch(t);
    CL_TEST_ASSERT(swtch != NULL);
    i = 0; 
    while(swtch) {
        if (i == 0) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s1"));
        if (i == 1) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s2"));
        if (i == 2) CL_TEST_FAIL();
        i++; 
        swtch = cl_switch_tok_get_next(swtch);
    }

    swtch = cl_cmd_tok_get_switch(t);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s1"));
    arg = cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)swtch);
    CL_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg1"));
        if (i == 1) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg2"));
        if (i == 2) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg3"));
        if (i == 3) CL_TEST_FAIL();
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    swtch = cl_switch_tok_get_next(swtch);
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)swtch, "-s2"));
    arg = cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)swtch);
    CL_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "sarg3"));
        if (i == 1) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "long switch argument"));
        if (i == 2) CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)arg, "-12.43"));
        if (i == 3) CL_TEST_FAIL();
        i++; 
        arg = cl_arg_tok_get_next(arg);
    }

    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_cmd_parser_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_cmd_parser_parse_parses_command_value,
        cl_cmd_parser_parse_parses_short_argument,
        cl_cmd_parser_parse_parses_long_argument,
        cl_cmd_parser_parse_parses_single_quotes,
        cl_cmd_parser_parse_parses_switch,
        cl_cmd_parser_parse_parses_numeric_argument,
        cl_cmd_parser_parses_non_numeric_switch,
        cl_cmd_parser_parse_arguments_parsed_in_correct_order,
        cl_cmd_parser_parse_switches_parsed_in_correct_order,
        cl_cmd_parser_parse_parses_switch_arguments_in_correct_order,
        cl_cmd_parser_parse_parses_command,
        cl_cmd_parser_parse_parses_trailing_quotes,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
