#include <stddef.h>
#include "ucmdtests.h"

static ucTestErr ucArgTokOwner_get_arg_returns_first_arg(ucTestGroup *p) {
    char cmd[] = "c\0a1\0a2\0\n";
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTokOwner_get_arg(cmd), "a1"));
    ucPASS();
}

static ucTestErr ucArgTokOwner_get_arg_returns_null(ucTestGroup *p) {
    char cmd[] = "cmd\0\n";
    ucTRUE(NULL == ucArgTokOwner_get_arg(cmd));
    ucPASS();
}

uc_TEST_GROUP(ucArgTokOwner, NULL,
    ucArgTokOwner_get_arg_returns_first_arg,
    ucArgTokOwner_get_arg_returns_null)
