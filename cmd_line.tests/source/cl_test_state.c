#include <stdlib.h>
#include "cl_test_state.h"

int cl_test_state_get_run_test_count(cl_test_state *p) {
    if (NULL == p) return 0;
    return p->run_test_count;
}

int cl_test_state_get_run_group_count(cl_test_state *p) {
    if (NULL == p) return 0;
    return p->run_group_count;
}

int cl_test_state_get_run_group_test_count(cl_test_state *p) {
    if (NULL == p) return 0;
    return p->run_group_test_count;
}

void cl_test_state_set_run_test_count(cl_test_state *p, int value) {
    if (NULL == p) return;
    p->run_test_count = value;
}

void cl_test_state_set_run_group_count(cl_test_state *p, int value) {
    if (NULL == p) return;
    p->run_group_count = value;
}

void cl_test_state_set_run_group_test_count(cl_test_state *p, int value) {
    if (NULL == p) return;
    p->run_group_test_count = value;
}

cl_test_state *cl_test_state_init(cl_test_state *p) {
    if (NULL == p) return NULL;

    cl_test_state_reset(p);

    return p;
}

void cl_test_state_reset(cl_test_state *p) {
    if (NULL == p) return;

    p->run_test_count = 0;
    p->run_group_count = 0;
    p->run_group_test_count = 0;
}
