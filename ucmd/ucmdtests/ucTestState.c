#include <stdlib.h>
#include "ucTestState.h"

int ucTestState_get_run_test_count(ucTestState *p) {
    if (NULL == p) return 0;
    return p->run_test_count;
}

int ucTestState_get_run_group_count(ucTestState *p) {
    if (NULL == p) return 0;
    return p->run_group_count;
}

int ucTestState_get_run_group_test_count(ucTestState *p) {
    if (NULL == p) return 0;
    return p->run_group_test_count;
}

void ucTestState_set_run_test_count(ucTestState *p, int value) {
    if (NULL == p) return;
    p->run_test_count = value;
}

void ucTestState_set_run_group_count(ucTestState *p, int value) {
    if (NULL == p) return;
    p->run_group_count = value;
}

void ucTestState_set_run_group_test_count(ucTestState *p, int value) {
    if (NULL == p) return;
    p->run_group_test_count = value;
}

ucTestState *ucTestState_init(ucTestState *p) {
    if (NULL == p) return NULL;

    ucTestState_reset(p);

    return p;
}

void ucTestState_reset(ucTestState *p) {
    if (NULL == p) return;

    p->run_test_count = 0;
    p->run_group_count = 0;
    p->run_group_test_count = 0;
}
