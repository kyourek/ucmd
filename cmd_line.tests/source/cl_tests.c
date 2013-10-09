#include <stdio.h>
#include <stdlib.h>
#include "cl_arg_opt_tests.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_arg_tok_tests.h"
#include "cl_bool_tests.h"
#include "cl_cmd_parser_tests.h"
#include "cl_cmd_tok_tests.h"
#include "cl_opt_tests.h"
#include "cl_switch_opt_tests.h"
#include "cl_switch_tok_tests.h"
#include "cl_tests_p.h"
#include "cl_tok_tests.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_cmd_line_opt_tests.h"
#include "cl_cmd_line_toks_tests.h"
#include "cl_cmd_line_tests.h"

static cl_tests_err cl_tests_group_run_callback_invoke(cl_tests_run_group_callback_func *func, void *state) {
    if (NULL != func) return func(state);
    return CL_TESTS_ERR_NONE;
}

cl_tests *cl_tests_running_instance = NULL;

cl_tests *cl_tests_get_instance() {
    static cl_tests instance;
    static cl_tests *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->run_count = 0;
        p->run_group_count = 0;
        p->assertions_made = 0;
        p->group_run_count = 0;
        p->group_assertions_made = 0;
        p->unattended = CL_FALSE;
        p->err_returned = CL_TESTS_ERR_NONE;

        p->run_test_count = 0;
        p->run_group_count = 0;
        p->run_group_test_count = 0;

        p->print_func = NULL;
        p->print_state = NULL;

        p->exit_func = NULL;
        p->exit_state = NULL;
    }
    return p;
}

int cl_tests_get_run_count(cl_tests *p) {
    if (NULL == p) return 0;
    return p->run_count;
}

int cl_tests_get_run_group_count(cl_tests *p) {
    if (NULL == p) return 0;
    return p->run_group_count;
}

int cl_tests_get_assertions_made(cl_tests *p) {
    if (NULL == p) return 0;
    return p->assertions_made;
}

int cl_tests_get_group_run_count(cl_tests *p) {
    if (NULL == p) return 0;
    return p->group_run_count;
}

int cl_tests_get_group_assertions_made(cl_tests *p) {
    if (NULL == p) return 0;
    return p->group_assertions_made;
}

cl_bool cl_tests_get_unattended(cl_tests *p) {
    if (NULL == p) return CL_FALSE;
    return p->unattended;
}

void cl_tests_set_unattended(cl_tests *p, cl_bool value) {
    if (NULL == p) return;
    p->unattended = value;
}

cl_tests_err cl_tests_get_err_returned(cl_tests *p) {
    if (NULL == p) return CL_TESTS_ERR_NONE;
    return p->err_returned;
}

cl_tests_err cl_tests_run(cl_tests *p) {
    if (NULL == p) return -1;

    p->run_count = 0;
    p->run_group_count = 0;
    p->assertions_made = 0;
    p->group_run_count = 0;
    p->group_assertions_made = 0;
    p->err_returned = CL_TESTS_ERR_NONE;

    cl_tests_running_instance = p;

    CL_TESTS_RUN_GROUP(cl_arg_opt_tests);
    CL_TESTS_RUN_GROUP(cl_arg_tok_owner_tests);
    CL_TESTS_RUN_GROUP(cl_arg_tok_tests);
    CL_TESTS_RUN_GROUP(cl_bool_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_parser_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_tok_tests);
    CL_TESTS_RUN_GROUP(cl_opt_tests);
    CL_TESTS_RUN_GROUP(cl_switch_opt_tests);
    CL_TESTS_RUN_GROUP(cl_switch_tok_tests);
    CL_TESTS_RUN_GROUP(cl_tok_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_app_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_opt_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_toks_tests);

    return p->err_returned;
}

cl_tests_err cl_tests_main(cl_tests *p) {
    char str[50];
    cl_tests_exit_func *exit;
    cl_tests_print_func *print;

    if (NULL == p) return -1;
    exit = p->exit_func;
    print = p->print_func;

    cl_tests_err err = cl_tests_run(p);

    if (NULL != print) {
        if (err) {
            sprintf(str, "Test failed in group %d.\n", cl_tests_get_run_group_count(p));
            print(str, p->print_state);
        }
        else {
            sprintf(str, "%s", "All tests passed.\n");
            print(str);
        }
        sprintf(str, "Tests run: %d\n", cl_tests_get_run_count(p));
        print(str);
        sprintf(str, "Assertions made: %d\n", cl_tests_get_assertions_made(p));
        print(str);

        if (!cl_tests_get_unattended(p)) {
            if (NULL != exit) {
                print("Exit?\n");
                while (!exit()) {
                    continue;
                }
            }
        }
    }

    return err;
}

void cl_tests_set_print_func(cl_tests *p, cl_tests_print_func *value) {
    if (NULL == p) return;
    p->print_func = value;
}

void cl_tests_set_print_state(cl_tests *p, void *value) {
    if (NULL == p) return;
    p->print_state = value;
}

void cl_tests_set_exit_func(cl_tests *p, cl_tests_exit_func *value) {
    if (NULL == p) return;
    p->exit_func = value;
}

void cl_tests_set_exit_state(cl_tests *p, void *value) {
    if (NULL == p) return;
    p->exit_state = value;
}

void cl_tests_print(cl_tests *p, const char *str) {
    if (NULL == p) return;
    if (NULL == p->print_func) return;
    p->print_func(str, p->print_state);
}

cl_bool cl_tests_exit(cl_tests *p) {
    if (NULL == p) return CL_TRUE;
    if (NULL == p->exit_func) return CL_TRUE;
    return p->exit_func(p->exit_state);
}

cl_tests_err cl_tests_run_group(cl_tests *p, cl_test_group *group) {
    cl_tests_err err, callback_err;
    cl_test_group_test_func **tests;

    if (NULL == p) return -1;

    tests = cl_test_group_get_tests(group);
    if (NULL == tests) return -2;

    p->run_group_test_count = 0;

    callback_err = cl_test_group_before_all_tests(group);
    if (callback_err) return callback_err;

    err = CL_TESTS_ERR_NONE;
    for (; *tests; tests++) {

        callback_err = cl_test_group_before_each_test(group);
        if (callback_err) return callback_err;

        err = (*tests)(group);

        callback_err = cl_tests_group_run_callback_invoke(after_each_test, state);
        if (callback_err) return callback_err;

        if (err) break;

        p->run_group_test_count++;
    }

    callback_err = cl_tests_group_run_callback_invoke(after_all_tests, state);
    if (callback_err) return callback_err;

    if (err) return err;

    p->run_group_count++;

    return CL_TESTS_ERR_NONE;
}
