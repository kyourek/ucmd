#include <stdlib.h>
#include "cl_test_group.h"

cl_tests_err cl_test_group_before_all_tests(cl_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_all_tests) return -2;
    return p->before_all_tests(p);
}

cl_tests_err cl_test_group_base_before_all_tests(cl_test_group *p) {
    return CL_TESTS_ERR_NONE;
}

cl_tests_err cl_test_group_after_all_tests(cl_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_all_tests) return -2;
    return p->after_all_tests(p);
}

cl_tests_err cl_test_group_base_after_all_tests(cl_test_group *p) {
    return CL_TESTS_ERR_NONE;
}

cl_tests_err cl_test_group_before_each_test(cl_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->before_each_test) return -2;
    return p->before_each_test(p);
}

cl_tests_err cl_test_group_base_before_each_test(cl_test_group *p) {
    return CL_TESTS_ERR_NONE;
}

cl_tests_err cl_test_group_after_each_test(cl_test_group *p) {
    if (NULL == p) return -1;
    if (NULL == p->after_each_test) return -2;
    return p->after_each_test(p);
}

cl_tests_err cl_test_group_base_after_each_test(cl_test_group *p) {
    return CL_TESTS_ERR_NONE;
}

cl_test_group_test_func **cl_test_group_get_tests(cl_test_group *p) {
    if (NULL == p) return NULL;
    return p->tests;
}

cl_test_group *cl_test_group_init(
    cl_test_group *p, 
    cl_test_group_callback_func *before_all_tests, 
    cl_test_group_callback_func *after_all_tests, 
    cl_test_group_callback_func *before_each_test, 
    cl_test_group_callback_func *after_each_test,
    cl_test_group_test_func **tests
) {
    static cl_test_group_test_func *base_tests[] = { NULL };

    if (NULL == p) return NULL;

    p->tests = NULL == tests ? base_tests : tests;
    p->before_all_tests = NULL == before_all_tests ? cl_test_group_base_before_all_tests : before_all_tests;
    p->after_all_tests = NULL == after_all_tests ? cl_test_group_base_after_all_tests : after_all_tests;
    p->before_each_test = NULL == before_each_test ? cl_test_group_base_before_each_test : before_each_test;
    p->after_each_test = NULL == after_each_test ? cl_test_group_base_after_each_test : after_each_test;

    return p;
}

cl_tests_err cl_test_group_run(cl_test_group *p, cl_test_state *state) {
    
    cl_tests_err err, callback_err;
    cl_test_group_test_func **tests;

    if (NULL == p) return -1;

    tests = cl_test_group_get_tests(p);
    if (NULL == tests) return -2;

    cl_test_state_set_run_group_test_count(state, 0);

    callback_err = cl_test_group_before_all_tests(p);
    if (callback_err) return callback_err;

    err = CL_TESTS_ERR_NONE;
    for (; *tests; tests++) {

        callback_err = cl_test_group_before_each_test(p);
        if (callback_err) return callback_err;

        err = (*tests)(p);

        callback_err = cl_test_group_after_each_test(p);
        if (callback_err) return callback_err;

        if (err) break;

        cl_test_state_set_run_group_test_count(state, cl_test_state_get_run_group_test_count(state) + 1);
    }

    callback_err = cl_test_group_after_all_tests(p);
    if (callback_err) return callback_err;

    if (err) return err;

    cl_test_state_set_run_group_count(state, cl_test_state_get_run_group_count(state) + 1);

    return CL_TESTS_ERR_NONE;
}
