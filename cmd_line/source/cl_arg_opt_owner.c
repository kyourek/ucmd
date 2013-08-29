#include <stdlib.h>
#include <stdarg.h>
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"
#include "cl_arg_opt_owner.h"
#include "cl_arg_opt_owner_p.h"
#include "cl_opt.h"
#include "cl_opt_p.h"

cl_arg_opt *cl_arg_opt_owner_get_arg_opt(cl_arg_opt_owner *p) {
    if (NULL == p) return NULL;
    return p->arg_opt;
}

cl_arg_opt_owner *cl_arg_opt_owner_init(cl_arg_opt_owner *p, const char *name, const char *desc, CL_BOOL is_required, cl_arg_opt *arg_opt) {
    if (NULL == p) return NULL;
    if (NULL == cl_opt_init((cl_opt*)p, name, desc, is_required)) return NULL;
    p->arg_opt = arg_opt;
    return p;
}
