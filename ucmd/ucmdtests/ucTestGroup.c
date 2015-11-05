#include <stdlib.h>
#include "ucmdtests.h"

static int setup(ucTestGroup *p) {
    assert(p);
    return p->setup
        ? p->setup(p)
        : 0;
}

static int prior(ucTestGroup *p) {
    assert(p);
    return p->prior
        ? p->prior(p)
        : 0;
}

static int after(ucTestGroup *p) {
    assert(p);
    return p->after
        ? p->after(p)
        : 0;
}

static void print(ucTestGroup *p, const char *format, ...) {
    va_list va;
    ucTestRunner *test_runner;
    assert(p);
    test_runner = p->test_runner;
    va_start(va, format);
    if (test_runner) {
        ucTestRunner_print(test_runner, format, va);
    }
    va_end(va);
}

static void inc_test_count(ucTestGroup *p, ucBool test_passed) {
    ucTestRunner *test_runner;
    assert(p);
    test_runner = p->test_runner;
    if (test_runner) {
        ucTestRunner_inc_test_count(test_runner, test_passed);
    }
}

void ucTestGroup_set_name(ucTestGroup *p, const char *value) {
    assert(p);
    p->name = value;
}

const char *ucTestGroup_get_name(ucTestGroup *p) {
    assert(p);
    return p->name;
}

void ucTestGroup_set_setup(ucTestGroup *p, ucTestGroup_TestFunc *value) {
    assert(p);
    p->setup = value;
}

void ucTestGroup_set_test(ucTestGroup *p, ucTestGroup_TestFunc **value) {
    assert(p);
    p->test = value;
}

void ucTestGroup_set_test_runner(ucTestGroup *p, ucTestRunner *value) {
    assert(p);
    p->test_runner = value;
}

ucTestRunner *ucTestGroup_get_test_runner(ucTestGroup *p) {
    assert(p);
    return p->test_runner;
}

void ucTestGroup_setup_test(ucTestGroup *p, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after) {
    assert(p);
    p->prior = prior;
    p->after = after;
}

ucTestErr ucTestGroup_run(ucTestGroup *p) {
    int err;
    ucTestGroup_TestFunc **test;

    assert(p);

    err = setup(p);
    if (err) return err;

    for (test = p->test; *test; test++) {

        err = prior(p);
        if (err) return err;

        err = (*test)(p);
        inc_test_count(p, err ? ucBool_false : ucBool_true);

        err = after(p);
        if (err) return err;
    }

    return 0;
}
