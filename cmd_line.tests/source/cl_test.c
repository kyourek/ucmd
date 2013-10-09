#include <stdio.h>
#include <stdlib.h>
#include "cl_test.h"

static void cl_test_print(cl_test *p, const char *str) {
    if (NULL == p) return;
    if (NULL == p->print_func) return;
    p->print_func(str, p->print_state);
}

static cl_bool cl_test_exit(cl_test *p) {
    if (NULL == p) return CL_TRUE;
    if (NULL == p->exit_func) return CL_TRUE;
    return p->exit_func(p->exit_state);
}

void cl_test_set_print_func(cl_test *p, cl_test_print_func *value) {
    if (NULL == p) return;
    p->print_func = value;
}

void cl_test_set_print_state(cl_test *p, void *value) {
    if (NULL == p) return;
    p->print_state = value;
}

void cl_test_set_exit_func(cl_test *p, cl_test_exit_func *value) {
    if (NULL == p) return;
    p->exit_func = value;
}

void cl_test_set_exit_state(cl_test *p, void *value) {
    if (NULL == p) return;
    p->exit_state = value;
}

cl_test_group **cl_test_get_groups(cl_test *p) {
    if (NULL == p) return NULL;
    return p->groups;
}

cl_test_state *cl_test_get_state(cl_test *p) {
    if (NULL == p) return NULL;
    return &p->state;
}

cl_tests_err cl_test_run(cl_test *p) {
    char str[50];
    cl_tests_err err;
    cl_test_state *state;

    cl_test_group **groups = cl_test_get_groups(p);
    if (NULL == groups) return -1;

    state = cl_test_get_state(p);
    cl_test_state_reset(state);

    err = CL_TESTS_ERR_NONE;
    for (; *groups; groups++) {

        err = cl_test_group_run(*groups, state);
        if (err) {

            sprintf(str, "Group %d failed.\n", cl_test_state_get_run_group_count(state));
            cl_test_print(p, str);

            sprintf(str, "%d group tests run.\n", cl_test_state_get_run_group_test_count(state));
            cl_test_print(p, str);

            break;
        }
    }

    if (!err) {
        cl_test_print(p, "All tests passed.\n");
    }

    sprintf(str, "%d total tests run.\n", cl_test_state_get_run_test_count(state));
    cl_test_print(p, str);

    cl_test_print(p, "Exit?\n");
    while (!cl_test_exit(p));

    return err;
}

cl_test *cl_test_init(cl_test *p, cl_test_group **groups) {
    
    static cl_test_group *base_groups[] = { NULL };

    if (NULL == p) return NULL;
    if (NULL == cl_test_state_init(&p->state)) return NULL;

    p->exit_func = NULL;
    p->exit_state = NULL;
    p->print_func = NULL;
    p->print_state = NULL;
    p->groups = NULL == groups ? base_groups : groups;

    return p;
}
