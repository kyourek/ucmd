#include <stdio.h>
#include <stdlib.h>
#include "ucmdtests.h"

static ucCmdParser *get_cmd_parser(void) {
    return ucCmdParser_get_instance();
}

static ucCmdTok *parse_cmd(char *cmd) {
    return ucCmdParser_parse(get_cmd_parser(), cmd);
}

static ucTestErr ucCmdParser_parse_parses_command_value(ucTestGroup *p) {
    char cmd[20] = "command_name";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(NULL != t);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)t, cmd));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_short_argument(ucTestGroup *p) {
    char cmd[24] = "command short_arg";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "short_arg"));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_long_argument(ucTestGroup *p) {
    char cmd[31] = "command \"long argument\"";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "long argument"));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_single_quotes(ucTestGroup *p) {
    char cmd[47] = "command 'long argument'";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "long argument"));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_switch(ucTestGroup *p) {
    char cmd[19] = "command -switch";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-switch"));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_numeric_argument(ucTestGroup *p) {
    char cmd[38] = "command -12.34";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(NULL == ucCmdTok_get_switch(t));
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(t), "-12.34"));
    ucPASS();
}

static ucTestErr ucCmdParser_parses_non_numeric_switch(ucTestGroup *p) {
    char cmd[23] = "command -32.4.0";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(NULL == ucCmdTok_get_arg(t));
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-32.4.0"));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_arguments_parsed_in_correct_order(ucTestGroup *p) {
    int i = 0;
    char buf[10] = { '\0' };
    char cmd[81] = "command arg1 arg2 arg3 arg4 arg5 arg6 -s1 arg arg1 -s2 arg1 arg2 arg3";
    ucArgTok *arg;

    ucCmdTok *t = parse_cmd(cmd);

    i = 1;
    arg = ucCmdTok_get_arg(t); 
    while (arg) {
        sprintf(buf, "arg%d", i);
        ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, buf));
        arg = ucArgTok_get_next(arg);
        if (arg) i++;
    }
    ucTRUE(6 == i);
    ucPASS();
}

static ucTestErr ucCmdParser_parse_switches_parsed_in_correct_order(ucTestGroup *p) {
    int i = 0;
    char buf[10];
    char cmd[76] = "command arg1 arg2 -sw1 arg1 arg2 -sw2 -sw3 arg1 arg2 -sw4";
    ucSwitchTok *swtch;
    
    ucCmdTok *t = parse_cmd(cmd);

    swtch = ucCmdTok_get_switch(t);
    i = 1;
    while (swtch) {
        sprintf(buf, "-sw%d", i);
        ucTRUE(ucBool_true == ucTok_equals((ucTok*)swtch, buf));
        swtch = ucSwitchTok_get_next(swtch);
        if (swtch) i++;
    }
    ucTRUE(4 == i);
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_switch_arguments_in_correct_order(ucTestGroup *p) {
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
        ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, buf));
        arg = ucArgTok_get_next(arg);
        if (arg) i++;
    }
    ucTRUE(i == 5);
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_trailing_quotes(ucTestGroup *p) {
    char cmd[38] = "cmd-name -s 'quoted \"arg\"'";
    ucCmdTok *t = parse_cmd(cmd);
    ucTRUE(ucTok_equals((ucTok*)t, "cmd-name"));
    ucTRUE(ucTok_equals((ucTok*)ucCmdTok_get_switch(t), "-s"));
    ucTRUE(ucTok_equals((ucTok*)ucSwitchTok_get_arg(ucCmdTok_get_switch(t)), "quoted \"arg\""));
    ucPASS();
}

static ucTestErr ucCmdParser_parse_parses_command(ucTestGroup *p) {
    int i = 0;
    char cmd[139] = "somecmd 'this is a long argument' shortarg -73.452 -s1 sarg1 sarg2 sarg3 -s2 sarg3 'long switch argument' -12.43";
    ucArgTok *arg = NULL;
    ucSwitchTok *swtch = NULL;
    
    ucCmdTok *t = parse_cmd(cmd);

    ucTRUE(NULL != t);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)t, "somecmd"));
    ucTRUE(3 == ucArgTok_count(ucCmdTok_get_arg(t)));
    ucTRUE(2 == ucSwitchTok_count(ucCmdTok_get_switch(t)));

    arg = ucCmdTok_get_arg(t);
    ucTRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "this is a long argument"));
        if (i == 1) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "shortarg"));
        if (i == 2) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "-73.452"));
        if (i == 3) ucFAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }

    swtch = ucCmdTok_get_switch(t);
    ucTRUE(swtch != NULL);
    i = 0; 
    while(swtch) {
        if (i == 0) ucTRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s1"));
        if (i == 1) ucTRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s2"));
        if (i == 2) ucFAIL();
        i++; 
        swtch = ucSwitchTok_get_next(swtch);
    }

    swtch = ucCmdTok_get_switch(t);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s1"));
    arg = ucArgTokOwner_get_arg((ucArgTokOwner*)swtch);
    ucTRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg1"));
        if (i == 1) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg2"));
        if (i == 2) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg3"));
        if (i == 3) ucFAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }

    swtch = ucSwitchTok_get_next(swtch);
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)swtch, "-s2"));
    arg = ucArgTokOwner_get_arg((ucArgTokOwner*)swtch);
    ucTRUE(arg != NULL);
    i = 0; 
    while(arg) {
        if (i == 0) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "sarg3"));
        if (i == 1) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "long switch argument"));
        if (i == 2) ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "-12.43"));
        if (i == 3) ucFAIL();
        i++; 
        arg = ucArgTok_get_next(arg);
    }

    ucPASS();
}

static ucTestErr ucCmdParser_parse_allows_empty_double_quotes(ucTestGroup *p) {
    char cmd[50] = "cmd -s \"\"";
    ucArgTok *arg = NULL;
    ucSwitchTok *swtch = NULL;
    
    ucCmdTok *t = parse_cmd(cmd);

    swtch = ucCmdTok_get_switch(t);
    arg = ucSwitchTok_get_arg(swtch);

    ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "\"\""));

    ucPASS();
}

static ucTestErr ucCmdParser_parse_allows_empty_single_quotes(ucTestGroup *p) {
    char cmd[50] = "c ''";
    ucArgTok *arg = NULL;
    ucCmdTok *t = parse_cmd(cmd);

    arg = ucCmdTok_get_arg(t);

    ucTRUE(ucBool_true == ucTok_equals((ucTok*)arg, "''"));

    ucPASS();
}

ucTestGroup *ucCmdParser_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdParser_parse_parses_command_value,
        ucCmdParser_parse_parses_short_argument,
        ucCmdParser_parse_parses_long_argument,
        ucCmdParser_parse_parses_single_quotes,
        ucCmdParser_parse_parses_switch,
        ucCmdParser_parse_parses_numeric_argument,
        ucCmdParser_parses_non_numeric_switch,
        ucCmdParser_parse_arguments_parsed_in_correct_order,
        ucCmdParser_parse_switches_parsed_in_correct_order,
        ucCmdParser_parse_parses_switch_arguments_in_correct_order,
        ucCmdParser_parse_parses_command,
        ucCmdParser_parse_parses_trailing_quotes,
        ucCmdParser_parse_allows_empty_double_quotes,
        ucCmdParser_parse_allows_empty_single_quotes,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
