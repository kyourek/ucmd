#include <stdlib.h>
#include "ucTestGroup.h"

ucTestErr ucTestGroup_before_all_tests(ucTestGroup *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_all_tests) return -2;
    return p->before_all_tests(p);
}

ucTestErr ucTestGroup_base_before_all_tests(ucTestGroup *p) {
    return ucTestErr_NONE;
}

ucTestErr ucTestGroup_after_all_tests(ucTestGroup *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_all_tests) return -2;
    return p->after_all_tests(p);
}

ucTestErr ucTestGroup_base_after_all_tests(ucTestGroup *p) {
    return ucTestErr_NONE;
}

ucTestErr ucTestGroup_before_each_test(ucTestGroup *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_each_test) return -2;
    return p->before_each_test(p);
}

ucTestErr ucTestGroup_base_before_each_test(ucTestGroup *p) {
    return ucTestErr_NONE;
}

ucTestErr ucTestGroup_after_each_test(ucTestGroup *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_each_test) return -2;
    return p->after_each_test(p);
}

ucTestErr ucTestGroup_base_after_each_test(ucTestGroup *p) {
    return ucTestErr_NONE;
}

ucTestGroup_test_func **ucTestGroup_get_tests(ucTestGroup *p) {
    if (NULL == p) return NULL;
    return p->tests;
}

ucTestGroup *ucTestGroup_init(
    ucTestGroup *p, 
    ucTestGroup_callback_func *before_all_tests, 
    ucTestGroup_callback_func *after_all_tests, 
    ucTestGroup_callback_func *before_each_test, 
    ucTestGroup_callback_func *after_each_test,
    ucTestGroup_test_func **tests
) {
    static ucTestGroup_test_func *base_tests[] = { NULL };

    if (NULL == p) return NULL;

    p->tests = NULL == tests ? base_tests : tests;
    p->before_all_tests = NULL == before_all_tests ? ucTestGroup_base_before_all_tests : before_all_tests;
    p->after_all_tests = NULL == after_all_tests ? ucTestGroup_base_after_all_tests : after_all_tests;
    p->before_each_test = NULL == before_each_test ? ucTestGroup_base_before_each_test : before_each_test;
    p->after_each_test = NULL == after_each_test ? ucTestGroup_base_after_each_test : after_each_test;

    return p;
}

ucTestErr ucTestGroup_run(ucTestGroup *p, ucTestState *state) {
    
    ucTestErr err, callback_err;
    ucTestGroup_test_func **tests;

    if (NULL == p) return -1;

    tests = ucTestGroup_get_tests(p);
    if (NULL == tests) return -2;

    ucTestState_set_run_group_test_count(state, 0);

    callback_err = ucTestGroup_before_all_tests(p);
    if (callback_err) return callback_err;

    err = ucTestErr_NONE;
    for (; *tests; tests++) {

        callback_err = ucTestGroup_before_each_test(p);
        if (callback_err) return callback_err;

        err = (*tests)(p);

        callback_err = ucTestGroup_after_each_test(p);
        if (callback_err) return callback_err;

        if (err) break;

        ucTestState_set_run_test_count(state, ucTestState_get_run_test_count(state) + 1);
        ucTestState_set_run_group_test_count(state, ucTestState_get_run_group_test_count(state) + 1);
    }

    callback_err = ucTestGroup_after_all_tests(p);
    if (callback_err) return callback_err;

    if (err) return err;

    ucTestState_set_run_group_count(state, ucTestState_get_run_group_count(state) + 1);

    return ucTestErr_NONE;
}
