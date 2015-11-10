#include <stdio.h>
#include <stdlib.h>
#include "ucmdtests.h"

static ucParser *subject;

static ucCmdTok *parse_cmd(char *cmd) {
    return ucParser_parse(subject, cmd);
}

uc_TEST(prior)
    subject = ucParser_create();
uc_PASS

uc_TEST(after)
    ucParser_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucParser_parse_parses_command_value)
    char cmd[20] = "command_name";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(NULL != t);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)t, cmd));
uc_PASS

uc_TEST(ucParser_parse_parses_short_argument)
    char cmd[24] = "command short_arg";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "short_arg"));
uc_PASS

uc_TEST(ucParser_parse_parses_long_argument)
    char cmd[31] = "command \"long argument\"";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "long argument"));
uc_PASS

uc_TEST(ucParser_parse_parses_single_quotes)
    char cmd[47] = "command 'long argument'";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "long argument"));
uc_PASS

uc_TEST(ucParser_parse_parses_switch)
    char cmd[19] = "command -switch";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-switch"));
uc_PASS

uc_TEST(ucParser_parse_parses_numeric_argument)
    char cmd[38] = "command -12.34";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(NULL == ucCmdTok_get_switch(t));
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "-12.34"));
uc_PASS

uc_TEST(ucParser_parses_non_numeric_switch)
    char cmd[23] = "command -32.4.0";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(NULL == ucCmdTok_get_arg(t));
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-32.4.0"));
uc_PASS

uc_TEST(ucParser_parse_arguments_parsed_in_correct_order)
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[81] = "command arg1 arg2 arg3 arg4 arg5 arg6 -s1 arg arg1 -s2 arg1 arg2 arg3";
    ucArgTok *arg;

    ucCmdTok *t = parse_cmd(cmd);

    i = 1;
    arg = ucCmdTok_get_arg(t); 
    while (arg) {
        sprintf(buf, "arg%d", i);
        uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, buf));
        arg = ucArgTok_get_next(arg);
        if (arg) i++;
    }
    uc_TRUE(6 == i);
uc_PASS

uc_TEST(ucParser_parse_switches_parsed_in_correct_order)
    int i = 0;
    char buf[10];
    char cmd[76] = "command arg1 arg2 -sw1 arg1 arg2 -sw2 -sw3 arg1 arg2 -sw4";
    ucSwitchTok *swtch;
    
    ucCmdTok *t = parse_cmd(cmd);

    swtch = ucCmdTok_get_switch(t);
    i = 1;
    while (swtch) {
        sprintf(buf, "-sw%d", i);
        uc_TRUE(ucBool_true == ucTok_equals((ucTok*)swtch, buf));
        swtch = ucSwitchTok_get_next(swtch);
        if (swtch) i++;
    }
    uc_TRUE(4 == i);
uc_PASS

uc_TEST(ucParser_parse_parses_switch_arguments_in_correct_order)
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[94] = "command arg1 arg2 -sw1 arg1 arg2 arg3 arg4 arg5 -sw2 arg1 arg2";
    ucArgTok *arg;
    ucSwitchTok *swtch;

    ucCmdTok *t = parse_cmd(cmd);

    swtch = ucCmdTok_get_switch(t);
    arg = ucSwitchTok_get_arg(swtch);

    i = 1;
    while (arg) {
        sprintf(buf, "arg%d", i);
        uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, buf));
        arg = ucArgTok_get_next(arg);
        if (arg) i++;
    }
    uc_TRUE(i == 5);
uc_PASS

uc_TEST(ucParser_parse_parses_trailing_quotes)
    char cmd[38] = "cmd-name -s 'quoted \"arg\"'";
    ucCmdTok *t = parse_cmd(cmd);
    uc_TRUE(ucTok_equals((ucTok*)t, "cmd-name"));
    uc_TRUE(ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-s"));
    uc_TRUE(ucTok_equals((ucTok*)ucSwitchTok_get_arg(ucCmdTok_get_switch(t)), "quoted \"arg\""));
uc_PASS

uc_TEST(ucParser_parse_parses_command)
    int i = 0;
    char cmd[139] = "somecmd 'this is a long argument' shortarg -73.452 -s1 sarg1 sarg2 sarg3 -s2 sarg3 'long switch argument' -12.43";
    ucArgTok *arg = NULL;
    ucSwitchTok *swtch = NULL;
    
    ucCmdTok *t = parse_cmd(cmd);

    uc_TRUE(NULL != t);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)t, "somecmd"));
    uc_TRUE(3 == ucArgTok_count(ucCmdTok_get_arg(t)));
    uc_TRUE(2 == ucSwitchTok_count(ucCmdTok_get_switch(t)));

    arg = ucCmdTok_get_arg(t);
    uc_TRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "this is a long argument"));
        if (i == 1) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "shortarg"));
        if (i == 2) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "-73.452"));
        if (i == 3) uc_FAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }

    swtch = ucCmdTok_get_switch(t);
    uc_TRUE(swtch != NULL);
    i = 0; 
    while(swtch) {
        if (i == 0) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s1"));
        if (i == 1) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s2"));
        if (i == 2) uc_FAIL();
        i++; 
        swtch = ucSwitchTok_get_next(swtch);
    }

    swtch = ucCmdTok_get_switch(t);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s1"));
    arg = ucArgTokOwner_get_arg((ucArgTokOwner*)swtch);
    uc_TRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg1"));
        if (i == 1) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg2"));
        if (i == 2) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg3"));
        if (i == 3) uc_FAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }

    swtch = ucSwitchTok_get_next(swtch);
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s2"));
    arg = ucArgTokOwner_get_arg((ucArgTokOwner*)swtch);
    uc_TRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg3"));
        if (i == 1) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "long switch argument"));
        if (i == 2) uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "-12.43"));
        if (i == 3) uc_FAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }
uc_PASS

uc_TEST(ucParser_parse_allows_empty_double_quotes)
    char cmd[50] = "cmd -s \"\"";
    ucArgTok *arg = NULL;
    ucSwitchTok *swtch = NULL;
    
    ucCmdTok *t = parse_cmd(cmd);

    swtch = ucCmdTok_get_switch(t);
    arg = ucSwitchTok_get_arg(swtch);

    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "\"\""));
uc_PASS

uc_TEST(ucParser_parse_allows_empty_single_quotes)
    char cmd[50] = "c ''";
    ucArgTok *arg = NULL;
    ucCmdTok *t = parse_cmd(cmd);

    arg = ucCmdTok_get_arg(t);

    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)arg, "''"));
uc_PASS

uc_TEST_GROUP(ucParser, setup,
    ucParser_parse_parses_command_value,
    ucParser_parse_parses_short_argument,
    ucParser_parse_parses_long_argument,
    ucParser_parse_parses_single_quotes,
    ucParser_parse_parses_switch,
    ucParser_parse_parses_numeric_argument,
    ucParser_parses_non_numeric_switch,
    ucParser_parse_arguments_parsed_in_correct_order,
    ucParser_parse_switches_parsed_in_correct_order,
    ucParser_parse_parses_switch_arguments_in_correct_order,
    ucParser_parse_parses_command,
    ucParser_parse_parses_trailing_quotes,
    ucParser_parse_allows_empty_double_quotes,
    ucParser_parse_allows_empty_single_quotes)
