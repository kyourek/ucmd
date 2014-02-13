#include <stdlib.h>
#include "uc_test_group.h"

uc_test_err uc_test_group_before_all_tests(uc_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_all_tests) return -2;
    return p->before_all_tests(p);
}

uc_test_err uc_test_group_base_before_all_tests(uc_test_group *p) {
    return UC_TEST_ERR_NONE;
}

uc_test_err uc_test_group_after_all_tests(uc_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_all_tests) return -2;
    return p->after_all_tests(p);
}

uc_test_err uc_test_group_base_after_all_tests(uc_test_group *p) {
    return UC_TEST_ERR_NONE;
}

uc_test_err uc_test_group_before_each_test(uc_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_each_test) return -2;
    return p->before_each_test(p);
}

uc_test_err uc_test_group_base_before_each_test(uc_test_group *p) {
    return UC_TEST_ERR_NONE;
}

uc_test_err uc_test_group_after_each_test(uc_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_each_test) return -2;
    return p->after_each_test(p);
}

uc_test_err uc_test_group_base_after_each_test(uc_test_group *p) {
    return UC_TEST_ERR_NONE;
}

uc_test_group_test_func **uc_test_group_get_tests(uc_test_group *p) {
    if (NULL == p) return NULL;
    return p->tests;
}

uc_test_group *uc_test_group_init(
    uc_test_group *p, 
    uc_test_group_callback_func *before_all_tests, 
    uc_test_group_callback_func *after_all_tests, 
    uc_test_group_callback_func *before_each_test, 
    uc_test_group_callback_func *after_each_test,
    uc_test_group_test_func **tests
) {
    static uc_test_group_test_func *base_tests[] = { NULL };

    if (NULL == p) return NULL;

    p->tests = NULL == tests ? base_tests : tests;
    p->before_all_tests = NULL == before_all_tests ? uc_test_group_base_before_all_tests : before_all_tests;
    p->after_all_tests = NULL == after_all_tests ? uc_test_group_base_after_all_tests : after_all_tests;
    p->before_each_test = NULL == before_each_test ? uc_test_group_base_before_each_test : before_each_test;
    p->after_each_test = NULL == after_each_test ? uc_test_group_base_after_each_test : after_each_test;

    return p;
}

uc_test_err uc_test_group_run(uc_test_group *p, uc_test_state *state) {
    
    uc_test_err err, callback_err;
    uc_test_group_test_func **tests;

    if (NULL == p) return -1;

    tests = uc_test_group_get_tests(p);
    if (NULL == tests) return -2;

    uc_test_state_set_run_group_test_count(state, 0);

    callback_err = uc_test_group_before_all_tests(p);
    if (callback_err) return callback_err;

    err = UC_TEST_ERR_NONE;
    for (; *tests; tests++) {

        callback_err = uc_test_group_before_each_test(p);
        if (callback_err) return callback_err;

        err = (*tests)(p);

        callback_err = uc_test_group_after_each_test(p);
        if (callback_err) return callback_err;

        if (err) break;

        uc_test_state_set_run_test_count(state, uc_test_state_get_run_test_count(state) + 1);
        uc_test_state_set_run_group_test_count(state, uc_test_state_get_run_group_test_count(state) + 1);
    }

    callback_err = uc_test_group_after_all_tests(p);
    if (callback_err) return callback_err;

    if (err) return err;

    uc_test_state_set_run_group_count(state, uc_test_state_get_run_group_count(state) + 1);

    return UC_TEST_ERR_NONE;
}
