#include <stddef.h>
#include "ucmdtests.h"

uc_TEST(ucArgTokOwner_get_arg_returns_first_arg)
    char cmd[] = "c\0a1\0a2\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTokOwner_get_arg(cmd), "a1"));
uc_PASS

uc_TEST(ucArgTokOwner_get_arg_returns_null)
    char cmd[] = "cmd\0\n";
    uc_TRUE(NULL == ucArgTokOwner_get_arg(cmd));
uc_PASS

uc_TEST_GROUP(ucArgTokOwner, NULL,
    ucArgTokOwner_get_arg_returns_first_arg,
    ucArgTokOwner_get_arg_returns_null)
