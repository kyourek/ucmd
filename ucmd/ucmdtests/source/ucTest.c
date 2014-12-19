#include <stdio.h>
#include <stdlib.h>
#include "ucTest.h"

static void ucTest_print(ucTest *p, const char *str) {
    if (NULL == p) return;
    if (NULL == p->print_func) return;
    p->print_func(str, p->print_state);
}

static ucBool ucTest_exit(ucTest *p) {
    if (NULL == p) return ucBool_true;
    if (NULL == p->exit_func) return ucBool_true;
    return p->exit_func(p->exit_state);
}

void ucTest_set_print_func(ucTest *p, ucTest_print_func *value) {
    if (NULL == p) return;
    p->print_func = value;
}

void ucTest_set_print_state(ucTest *p, void *value) {
    if (NULL == p) return;
    p->print_state = value;
}

void ucTest_set_exit_func(ucTest *p, ucTest_exit_func *value) {
    if (NULL == p) return;
    p->exit_func = value;
}

void ucTest_set_exit_state(ucTest *p, void *value) {
    if (NULL == p) return;
    p->exit_state = value;
}

ucTestGroup **ucTest_get_groups(ucTest *p) {
    if (NULL == p) return NULL;
    return p->groups;
}

ucTestState *ucTest_get_state(ucTest *p) {
    if (NULL == p) return NULL;
    return &p->state;
}

const char *ucTest_get_label(ucTest *p) {
    if (NULL == p) return NULL;
    return p->label;
}

ucTestErr ucTest_run(ucTest *p) {
    char str[50];
    ucTestErr err;
    ucTestState *state;

    ucTestGroup **groups = ucTest_get_groups(p);
    if (NULL == groups) return -1;

    state = ucTest_get_state(p);
    ucTestState_reset(state);

    sprintf(str, "%s\n", ucTest_get_label(p));
    ucTest_print(p, str);

    err = ucTestErr_NONE;
    for (; *groups; groups++) {

        err = ucTestGroup_run(*groups, state);
        if (err) {

            sprintf(str, "Group %d failed.\n", ucTestState_get_run_group_count(state));
            ucTest_print(p, str);

            sprintf(str, "%d group tests run.\n", ucTestState_get_run_group_test_count(state));
            ucTest_print(p, str);

            break;
        }
    }

    if (!err) {
        ucTest_print(p, "All tests passed.\n");
    }

    sprintf(str, "%d total tests run.\n", ucTestState_get_run_test_count(state));
    ucTest_print(p, str);

    ucTest_print(p, "Exit?\n");
    while (!ucTest_exit(p));

    return err;
}

ucTest *ucTest_init(ucTest *p, const char *label, ucTestGroup **groups) {
    
    static ucTestGroup *base_groups[] = { NULL };

    if (NULL == p) return NULL;
    if (NULL == ucTestState_init(&p->state)) return NULL;

    p->label = label;
    p->exit_func = NULL;
    p->exit_state = NULL;
    p->print_func = NULL;
    p->print_state = NULL;
    p->groups = NULL == groups ? base_groups : groups;

    return p;
}
