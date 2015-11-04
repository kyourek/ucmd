#include <string.h>
#include "ucmd_internal.h"

ucBool uc_str_eq(const char *s1, const char *s2) {
    if (s1 == s2) return ucBool_true;
    if (!s1) return ucBool_false;
    if (!s2) return ucBool_false;
    return 0 == strcmp(s1, s2)
        ? ucBool_true
        : ucBool_false;
}
