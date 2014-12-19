#ifndef UC_TEST_STATE_H
#define UC_TEST_STATE_H

#include "uc_common.h"

typedef struct ucTestState ucTestState;

UC_EXPORTED int ucTestState_get_run_test_count(ucTestState *p);

UC_EXPORTED int ucTestState_get_run_group_count(ucTestState *p);

UC_EXPORTED int ucTestState_get_run_group_test_count(ucTestState *p);

UC_EXPORTED void ucTestState_set_run_test_count(ucTestState *p, int value);

UC_EXPORTED void ucTestState_set_run_group_count(ucTestState *p, int value);

UC_EXPORTED void ucTestState_set_run_group_test_count(ucTestState *p, int value);

UC_EXPORTED void ucTestState_reset(ucTestState *p);

UC_EXPORTED ucTestState *ucTestState_init(ucTestState *p);

struct ucTestState {
    int run_test_count;
    int run_group_count;
    int run_group_test_count;
};

#endif
