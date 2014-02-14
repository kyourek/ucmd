#include <stdio.h>
#include <stdlib.h>
#include "uc_arg_tok.h"
#include "uc_cmd_parser_p.h"
#include "uc_cmd_parser_tests.h"
#include "uc_cmd_tok.h"
#include "uc_switch_tok.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_cmd_parser *get_cmd_parser(void) {
    return uc_cmd_parser_get_instance();
}

static uc_cmd_tok *parse_cmd(char *cmd) {
    return uc_cmd_parser_parse(get_cmd_parser(), cmd);
}

static uc_test_err uc_cmd_parser_parse_parses_command_value(uc_test_group *p) {
    char cmd[20] = "command_name";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(NULL != t);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)t, cmd));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_short_argument(uc_test_group *p) {
    char cmd[24] = "command short_arg";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_arg(t), "short_arg"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_long_argument(uc_test_group *p) {
    char cmd[31] = "command \"long argument\"";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_arg(t), "long argument"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_single_quotes(uc_test_group *p) {
    char cmd[47] = "command 'long argument'";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_arg(t), "long argument"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_switch(uc_test_group *p) {
    char cmd[19] = "command -switch";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_switch(t), "-switch"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_numeric_argument(uc_test_group *p) {
    char cmd[38] = "command -12.34";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(NULL == uc_cmd_tok_get_switch(t));
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_arg(t), "-12.34"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parses_non_numeric_switch(uc_test_group *p) {
    char cmd[23] = "command -32.4.0";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(NULL == uc_cmd_tok_get_arg(t));
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_cmd_tok_get_switch(t), "-32.4.0"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_arguments_parsed_in_correct_order(uc_test_group *p) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[81] = "command arg1 arg2 arg3 arg4 arg5 arg6 -s1 arg arg1 -s2 arg1 arg2 arg3";
    uc_arg_tok *arg;

    uc_cmd_tok *t = parse_cmd(cmd);

    i = 1;
    arg = uc_cmd_tok_get_arg(t); 
    while (arg) {
        sprintf(buf, "arg%d", i);
        UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, buf));
        arg = uc_arg_tok_get_next(arg);
        if (arg) i++;
    }
    UC_TEST_ASSERT(6 == i);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_switches_parsed_in_correct_order(uc_test_group *p) {
    int i = 0;
    char buf[10];
    char cmd[76] = "command arg1 arg2 -sw1 arg1 arg2 -sw2 -sw3 arg1 arg2 -sw4";
    uc_switch_tok *swtch;
    
    uc_cmd_tok *t = parse_cmd(cmd);

    swtch = uc_cmd_tok_get_switch(t);
    i = 1;
    while (swtch) {
        sprintf(buf, "-sw%d", i);
        UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)swtch, buf));
        swtch = uc_switch_tok_get_next(swtch);
        if (swtch) i++;
    }
    UC_TEST_ASSERT(4 == i);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_switch_arguments_in_correct_order(uc_test_group *p) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[94] = "command arg1 arg2 -sw1 arg1 arg2 arg3 arg4 arg5 -sw2 arg1 arg2";
    uc_arg_tok *arg;
    uc_switch_tok *swtch;

    uc_cmd_tok *t = parse_cmd(cmd);

    swtch = uc_cmd_tok_get_switch(t);
    arg = uc_switch_tok_get_arg(swtch);

    i = 1;
    while (arg) {
        sprintf(buf, "arg%d", i);
        UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, buf));
        arg = uc_arg_tok_get_next(arg);
        if (arg) i++;
    }
    UC_TEST_ASSERT(i == 5);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_trailing_quotes(uc_test_group *p) {
    char cmd[38] = "cmd-name -s 'quoted \"arg\"'";
    uc_cmd_tok *t = parse_cmd(cmd);
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)t, "cmd-name"));
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)uc_cmd_tok_get_switch(t), "-s"));
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)uc_switch_tok_get_arg(uc_cmd_tok_get_switch(t)), "quoted \"arg\""));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_parses_command(uc_test_group *p) {
    int i = 0;
    char cmd[139] = "somecmd 'this is a long argument' shortarg -73.452 -s1 sarg1 sarg2 sarg3 -s2 sarg3 'long switch argument' -12.43";
    uc_arg_tok *arg = NULL;
    uc_switch_tok *swtch = NULL;
    
    uc_cmd_tok *t = parse_cmd(cmd);

    UC_TEST_ASSERT(NULL != t);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)t, "somecmd"));
    UC_TEST_ASSERT(3 == uc_arg_tok_count(uc_cmd_tok_get_arg(t)));
    UC_TEST_ASSERT(2 == uc_switch_tok_count(uc_cmd_tok_get_switch(t)));

    arg = uc_cmd_tok_get_arg(t);
    UC_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "this is a long argument"));
        if (i == 1) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "shortarg"));
        if (i == 2) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "-73.452"));
        if (i == 3) UC_TEST_FAIL();
        i++; 
        arg = uc_arg_tok_get_next(arg);
    }

    swtch = uc_cmd_tok_get_switch(t);
    UC_TEST_ASSERT(swtch != NULL);
    i = 0; 
    while(swtch) {
        if (i == 0) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)swtch, "-s1"));
        if (i == 1) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)swtch, "-s2"));
        if (i == 2) UC_TEST_FAIL();
        i++; 
        swtch = uc_switch_tok_get_next(swtch);
    }

    swtch = uc_cmd_tok_get_switch(t);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)swtch, "-s1"));
    arg = uc_arg_tok_owner_get_arg((uc_arg_tok_owner*)swtch);
    UC_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "sarg1"));
        if (i == 1) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "sarg2"));
        if (i == 2) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "sarg3"));
        if (i == 3) UC_TEST_FAIL();
        i++; 
        arg = uc_arg_tok_get_next(arg);
    }

    swtch = uc_switch_tok_get_next(swtch);
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)swtch, "-s2"));
    arg = uc_arg_tok_owner_get_arg((uc_arg_tok_owner*)swtch);
    UC_TEST_ASSERT(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "sarg3"));
        if (i == 1) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "long switch argument"));
        if (i == 2) UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "-12.43"));
        if (i == 3) UC_TEST_FAIL();
        i++; 
        arg = uc_arg_tok_get_next(arg);
    }

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_allows_empty_double_quotes(uc_test_group *p) {
    char cmd[50] = "cmd -s \"\"";
    uc_arg_tok *arg = NULL;
    uc_switch_tok *swtch = NULL;
    
    uc_cmd_tok *t = parse_cmd(cmd);

    swtch = uc_cmd_tok_get_switch(t);
    arg = uc_switch_tok_get_arg(swtch);

    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "\"\""));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_parser_parse_allows_empty_single_quotes(uc_test_group *p) {
    char cmd[50] = "c ''";
    uc_arg_tok *arg = NULL;
    uc_cmd_tok *t = parse_cmd(cmd);

    arg = uc_cmd_tok_get_arg(t);

    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)arg, "''"));

    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_parser_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_parser_parse_parses_command_value,
        uc_cmd_parser_parse_parses_short_argument,
        uc_cmd_parser_parse_parses_long_argument,
        uc_cmd_parser_parse_parses_single_quotes,
        uc_cmd_parser_parse_parses_switch,
        uc_cmd_parser_parse_parses_numeric_argument,
        uc_cmd_parser_parses_non_numeric_switch,
        uc_cmd_parser_parse_arguments_parsed_in_correct_order,
        uc_cmd_parser_parse_switches_parsed_in_correct_order,
        uc_cmd_parser_parse_parses_switch_arguments_in_correct_order,
        uc_cmd_parser_parse_parses_command,
        uc_cmd_parser_parse_parses_trailing_quotes,
        uc_cmd_parser_parse_allows_empty_double_quotes,
        uc_cmd_parser_parse_allows_empty_single_quotes,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
