#ifndef UC_TEST_STATE_H
#define UC_TEST_STATE_H

#include "uc_common.h"

typedef struct uc_test_state uc_test_state;

UC_EXPORTED int uc_test_state_get_run_test_count(uc_test_state *p);

UC_EXPORTED int uc_test_state_get_run_group_count(uc_test_state *p);

UC_EXPORTED int uc_test_state_get_run_group_test_count(uc_test_state *p);

UC_EXPORTED void uc_test_state_set_run_test_count(uc_test_state *p, int value);

UC_EXPORTED void uc_test_state_set_run_group_count(uc_test_state *p, int value);

UC_EXPORTED void uc_test_state_set_run_group_test_count(uc_test_state *p, int value);

UC_EXPORTED void uc_test_state_reset(uc_test_state *p);

UC_EXPORTED uc_test_state *uc_test_state_init(uc_test_state *p);

struct uc_test_state {
    int run_test_count;
    int run_group_count;
    int run_group_test_count;
};

#endif
