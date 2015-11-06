#include "ucmdtests.h"

static void va_print(ucTestRunner *p, const char *format, va_list va) {
    assert(p);
    if (p->print) {
        p->print(format, va, p->state);
    }
}

static void print(ucTestRunner *p, const char *format, ...) {
    va_list va;
    va_start(va, format);
    va_print(p, format, va);
    va_end(va);
}

static void close(ucTestRunner *p) {
    assert(p);
    if (p->close) {
        p->close(p->state);
    }
}

void ucTestRunner_set_print(ucTestRunner *p, ucTestRunner_PrintFunc *value) {
    assert(p);
    p->print = value;
}

void ucTestRunner_set_close(ucTestRunner *p, ucTestRunner_CloseFunc *value) {
    assert(p);
    p->close = value;
}

int ucTestRunner_run_group(ucTestRunner *p, ucTestGroup *group) {
    int err;
    ucTestGroup_TestFunc **tests;

    assert(p);

    err = ucTestGroup_setup(group);
    if (err) {
        print(p, "%s", "Group setup error occurred");
        return -1;
    }

    for (tests = ucTestGroup_get_test(group); *tests; tests++) {
        err = ucTestGroup_prior(group);
        if (err) {
            print(p, "%s", "Test prior error occurred");
            return -2;
        }

        err = (*tests)(group);
        err ? p->failed_test++ : p->passed_test++;

        err = ucTestGroup_after(group);
        if (err) {
            print(p, "%s", "Test after error occurred");
            return -3;
        }
    }

    return 0;
}

int ucTestRunner_run(ucTestRunner *p) {
    int err;
    ucTestGroup ***groups;
    
    assert(p);
    
    print(p, "Running %s tests...\n", p->name);

    for (groups = p->group; *groups; groups++) {
        err = ucTestRunner_run_group(p, **groups);
    }

    print(p, "%d tests run\n", (p->failed_test + p->passed_test));
    print(p, "%d tests passed\n", p->passed_test);
    print(p, "%d tests failed\n", p->failed_test);
    close(p);

    return -(p->failed_test);
}
