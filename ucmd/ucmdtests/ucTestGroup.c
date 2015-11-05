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

ucTestGroup *ucTestGroup_init(ucTestGroup *p) {
    assert(p);
    p->name = NULL;
    p->test = NULL;
    p->setup = NULL;
    p->prior = NULL;
    p->after = NULL;
    return p;
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

void ucTestGroup_setup_test(ucTestGroup *p, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after) {
    assert(p);
    p->prior = prior;
    p->after = after;
}

ucTestErr ucTestGroup_run(ucTestGroup *p, ucTestState *state) {
    ucTestErr err, callback_err;
    ucTestGroup_TestFunc **test;

    assert(p);

    callback_err = setup(p);
    if (callback_err) return callback_err;

    ucTestState_set_run_group_test_count(state, 0);

    err = 0;
    for (test = p->test; *test; test++) {

        callback_err = prior(p);
        if (callback_err) return callback_err;

        err = (*test)(p);

        callback_err = after(p);
        if (callback_err) return callback_err;

        if (err) break;

        ucTestState_set_run_test_count(state, ucTestState_get_run_test_count(state) + 1);
        ucTestState_set_run_group_test_count(state, ucTestState_get_run_group_test_count(state) + 1);
    }

    if (err) return err;

    ucTestState_set_run_group_count(state, ucTestState_get_run_group_count(state) + 1);

    return 0;
}
