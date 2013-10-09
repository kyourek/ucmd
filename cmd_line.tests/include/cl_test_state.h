#ifndef CL_TEST_STATE_H
#define CL_TEST_STATE_H

#include "cl_common.h"

typedef struct cl_test_state cl_test_state;

CL_EXPORTED int cl_test_state_get_run_test_count(cl_test_state *p);

CL_EXPORTED int cl_test_state_get_run_group_count(cl_test_state *p);

CL_EXPORTED int cl_test_state_get_run_group_test_count(cl_test_state *p);

CL_EXPORTED void cl_test_state_set_run_test_count(cl_test_state *p, int value);

CL_EXPORTED void cl_test_state_set_run_group_count(cl_test_state *p, int value);

CL_EXPORTED void cl_test_state_set_run_group_test_count(cl_test_state *p, int value);

CL_EXPORTED void cl_test_state_reset(cl_test_state *p);

CL_EXPORTED cl_test_state *cl_test_state_init(cl_test_state *p);

struct cl_test_state {
    int run_test_count;
    int run_group_count;
    int run_group_test_count;
};

#endif
