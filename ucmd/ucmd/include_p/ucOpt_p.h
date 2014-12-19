#ifndef UC_OPT_P_H
#define UC_OPT_P_H

#include "ucOpt.h"

struct ucOpt {
    const char *name;
    const char *desc;
    ucBool is_required;
};

extern const char *ucOpt_validation_err_invalid_argument_prefix;
extern const char *ucOpt_validation_err_invalid_switch_prefix;
extern const char *ucOpt_validation_err_invalid_switch_argument_prefix;

UC_EXPORTED ucOpt *ucOpt_init(ucOpt *p, const char *name, const char *desc, ucBool is_required);

#endif
