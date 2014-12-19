#ifndef UC_TEST_STATE_H
#define UC_TEST_STATE_H

#include "uc.h"

typedef struct ucTestState ucTestState;

uc_EXPORTED int ucTestState_get_run_test_count(ucTestState *p);

uc_EXPORTED int ucTestState_get_run_group_count(ucTestState *p);

uc_EXPORTED int ucTestState_get_run_group_test_count(ucTestState *p);

uc_EXPORTED void ucTestState_set_run_test_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_set_run_group_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_set_run_group_test_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_reset(ucTestState *p);

uc_EXPORTED ucTestState *ucTestState_init(ucTestState *p);

struct ucTestState {
    int run_test_count;
    int run_group_count;
    int run_group_test_count;
};

#endif
