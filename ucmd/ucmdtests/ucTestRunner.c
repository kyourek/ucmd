#include "ucmdtests.h"

static void print(ucTestRunner *p, const char *format, ...) {
    va_list va;
    va_start(va, format);
    ucTestRunner_print(p, format, va);
    va_end(va);
}

static void close(ucTestRunner *p) {
    assert(p);
    if (p->close) {
        p->close(p->state);
    }
}

void ucTestRunner_print(ucTestRunner *p, const char *format, va_list va) {
    assert(p);
    if (p->print) {
        p->print(format, va, p->state);
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

void ucTestRunner_inc_test_count(ucTestRunner *p, ucBool test_passed) {
    assert(p);
    if (test_passed) {
        p->passed_test_count++;
    }
    else {
        p->failed_test_count++;
    }
}

void ucTestRunner_run(ucTestRunner *p) {
    int err;
    ucTestGroup *group, ***groups;
    assert(p);
    
    print(p, "Running %s tests...\n", p->name);

    for (groups = p->group; *groups; groups++) {
        group = **groups;

        ucTestGroup_set_test_runner(group, p);
        err = ucTestGroup_run(group);
        ucTestGroup_set_test_runner(group, NULL);

        if (err) {
            print(p, "Error in group %s\n", group->name);
        }
    }

    print(p, "%d tests run\n", (p->failed_test_count + p->passed_test_count));
    print(p, "%d tests passed\n", p->passed_test_count);
    print(p, "%d tests failed\n", p->failed_test_count);
    close(p);
}
