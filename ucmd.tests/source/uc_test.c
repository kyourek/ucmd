#include <stdio.h>
#include <stdlib.h>
#include "uc_test.h"

static void uc_test_print(uc_test *p, const char *str) {
    if (NULL == p) return;
    if (NULL == p->print_func) return;
    p->print_func(str, p->print_state);
}

static uc_bool uc_test_exit(uc_test *p) {
    if (NULL == p) return UC_TRUE;
    if (NULL == p->exit_func) return UC_TRUE;
    return p->exit_func(p->exit_state);
}

void uc_test_set_print_func(uc_test *p, uc_test_print_func *value) {
    if (NULL == p) return;
    p->print_func = value;
}

void uc_test_set_print_state(uc_test *p, void *value) {
    if (NULL == p) return;
    p->print_state = value;
}

void uc_test_set_exit_func(uc_test *p, uc_test_exit_func *value) {
    if (NULL == p) return;
    p->exit_func = value;
}

void uc_test_set_exit_state(uc_test *p, void *value) {
    if (NULL == p) return;
    p->exit_state = value;
}

uc_test_group **uc_test_get_groups(uc_test *p) {
    if (NULL == p) return NULL;
    return p->groups;
}

uc_test_state *uc_test_get_state(uc_test *p) {
    if (NULL == p) return NULL;
    return &p->state;
}

const char *uc_test_get_label(uc_test *p) {
    if (NULL == p) return NULL;
    return p->label;
}

uc_test_err uc_test_run(uc_test *p) {
    char str[50];
    uc_test_err err;
    uc_test_state *state;

    uc_test_group **groups = uc_test_get_groups(p);
    if (NULL == groups) return -1;

    state = uc_test_get_state(p);
    uc_test_state_reset(state);

    sprintf(str, "%s\n", uc_test_get_label(p));
    uc_test_print(p, str);

    err = UC_TEST_ERR_NONE;
    for (; *groups; groups++) {

        err = uc_test_group_run(*groups, state);
        if (err) {

            sprintf(str, "Group %d failed.\n", uc_test_state_get_run_group_count(state));
            uc_test_print(p, str);

            sprintf(str, "%d group tests run.\n", uc_test_state_get_run_group_test_count(state));
            uc_test_print(p, str);

            break;
        }
    }

    if (!err) {
        uc_test_print(p, "All tests passed.\n");
    }

    sprintf(str, "%d total tests run.\n", uc_test_state_get_run_test_count(state));
    uc_test_print(p, str);

    uc_test_print(p, "Exit?\n");
    while (!uc_test_exit(p));

    return err;
}

uc_test *uc_test_init(uc_test *p, const char *label, uc_test_group **groups) {
    
    static uc_test_group *base_groups[] = { NULL };

    if (NULL == p) return NULL;
    if (NULL == uc_test_state_init(&p->state)) return NULL;

    p->label = label;
    p->exit_func = NULL;
    p->exit_state = NULL;
    p->print_func = NULL;
    p->print_state = NULL;
    p->groups = NULL == groups ? base_groups : groups;

    return p;
}
