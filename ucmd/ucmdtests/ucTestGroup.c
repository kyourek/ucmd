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

ucTestGroup *ucTestGroup_init(ucTestGroup *p, const char *name, ucTestGroup_TestFunc *setup, ucTestGroup_TestFunc **test) {
    assert(p);
    p->name = name;
    p->test = test;
    p->setup = setup;
    p->prior = NULL;
    p->after = NULL;
    return p;
}

void ucTestGroup_setup_test(ucTestGroup *p, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after) {
    assert(p);
    p->prior = prior;
    p->after = after;
}

ucTestErr ucTestGroup_run(ucTestGroup *p, ucTestState *state) {
    ucTestErr err, callback_err;
    ucTestGroup_TestFunc **tests;

    assert(p);

    tests = p->test;

    callback_err = setup(p);
    if (callback_err) return callback_err;

    ucTestState_set_run_group_test_count(state, 0);

    err = 0;
    for (; *tests; tests++) {

        callback_err = prior(p);
        if (callback_err) return callback_err;

        err = (*tests)(p);

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
