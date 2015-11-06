#include <stdlib.h>
#include "ucmdtests.h"

int ucTestGroup_setup(ucTestGroup *p) {
    assert(p);
    return p->setup
        ? p->setup(p)
        : 0;
}

int ucTestGroup_prior(ucTestGroup *p) {
    assert(p);
    return p->prior
        ? p->prior(p)
        : 0;
}

int ucTestGroup_after(ucTestGroup *p) {
    assert(p);
    return p->after
        ? p->after(p)
        : 0;
}

const char *ucTestGroup_get_name(ucTestGroup *p) {
    assert(p);
    return p->name;
}

ucTestGroup_TestFunc **ucTestGroup_get_test(ucTestGroup *p) {
    assert(p);
    return p->test;
}

void ucTestGroup_setup_test(ucTestGroup *p, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after) {
    assert(p);
    p->prior = prior;
    p->after = after;
}
