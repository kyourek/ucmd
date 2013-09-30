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

cl_tests *cl_tests_running_instance = NULL;

cl_tests *cl_tests_get_instance() {
    static cl_tests instance;
    static cl_tests *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->exit = NULL;
        p->print = NULL;
        p->run_count = 0;
        p->run_group_count = 0;
        p->assertions_made = 0;
        p->group_run_count = 0;
        p->group_assertions_made = 0;
        p->unattended = CL_FALSE;
        p->err_returned = CL_TESTS_NO_ERR;
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

CL_BOOL cl_tests_get_unattended(cl_tests *p) {
    if (NULL == p) return CL_FALSE;
    return p->unattended;
}

void cl_tests_set_unattended(cl_tests *p, CL_BOOL value) {
    if (NULL == p) return;
    p->unattended = value;
}

CL_TESTS_ERR cl_tests_get_err_returned(cl_tests *p) {
    if (NULL == p) return CL_TESTS_NO_ERR;
    return p->err_returned;
}

cl_tests_print_func *cl_tests_get_print(cl_tests *p) {
    if (NULL == p) return NULL;
    return p->print;
}

void cl_tests_set_print(cl_tests *p, cl_tests_print_func *value) {
    if (NULL == p) return;
    p->print = value;
}

cl_tests_exit_func *cl_tests_get_exit(cl_tests *p) {
    if (NULL == p) return NULL;
    return p->exit;
}

void cl_tests_set_exit(cl_tests *p, cl_tests_exit_func *value) {
    if (NULL == p) return;
    p->exit = value;
}

CL_TESTS_ERR cl_tests_run(cl_tests *p) {
    if (NULL == p) return -1;

    p->run_count = 0;
    p->run_group_count = 0;
    p->assertions_made = 0;
    p->group_run_count = 0;
    p->group_assertions_made = 0;
    p->err_returned = CL_TESTS_NO_ERR;

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

CL_TESTS_ERR cl_tests_main(cl_tests *p) {
    char str[50];
    cl_tests_exit_func *exit = cl_tests_get_exit(p);
    cl_tests_print_func *print = cl_tests_get_print(p);

    CL_TESTS_ERR err = cl_tests_run(p);

    if (NULL != print) {
        if (err) {
            sprintf(str, "Test failed in group %d.\n", cl_tests_get_run_group_count(p));
            print(str);
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
