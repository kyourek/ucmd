#ifndef UC_OPT_P_H
#define UC_OPT_P_H

#include "uc_opt.h"

struct uc_opt {
    const char *name;
    const char *desc;
    ucBool is_required;
};

extern const char *uc_opt_validation_err_invalid_argument_prefix;
extern const char *uc_opt_validation_err_invalid_switch_prefix;
extern const char *uc_opt_validation_err_invalid_switch_argument_prefix;

UC_EXPORTED uc_opt *uc_opt_init(uc_opt *p, const char *name, const char *desc, ucBool is_required);

#endif
