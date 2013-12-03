#include <stdlib.h>
#include "uc_test_state.h"

int uc_test_state_get_run_test_count(uc_test_state *p) {
    if (NULL == p) return 0;
    return p->run_test_count;
}

int uc_test_state_get_run_group_count(uc_test_state *p) {
    if (NULL == p) return 0;
    return p->run_group_count;
}

int uc_test_state_get_run_group_test_count(uc_test_state *p) {
    if (NULL == p) return 0;
    return p->run_group_test_count;
}

void uc_test_state_set_run_test_count(uc_test_state *p, int value) {
    if (NULL == p) return;
    p->run_test_count = value;
}

void uc_test_state_set_run_group_count(uc_test_state *p, int value) {
    if (NULL == p) return;
    p->run_group_count = value;
}

void uc_test_state_set_run_group_test_count(uc_test_state *p, int value) {
    if (NULL == p) return;
    p->run_group_test_count = value;
}

uc_test_state *uc_test_state_init(uc_test_state *p) {
    if (NULL == p) return NULL;

    uc_test_state_reset(p);

    return p;
}

void uc_test_state_reset(uc_test_state *p) {
    if (NULL == p) return;

    p->run_test_count = 0;
    p->run_group_count = 0;
    p->run_group_test_count = 0;
}
