#include <stdlib.h>
#include "cl_common.h"
#include "cl_opt.h"
#include "cl_opt_p.h"

const char *cl_opt_get_name(cl_opt *p) {
    if (NULL == p) return NULL;
    return p->name;
}

const char *cl_opt_get_desc(cl_opt *p) {
    if (NULL == p) return NULL;
    return p->desc;
}

CL_BOOL cl_opt_is_required(cl_opt *p) {
    if (NULL == p) return CL_FALSE;
    return p->is_required;
}

cl_opt *cl_opt_init(cl_opt *p, const char *name, const char *desc, CL_BOOL is_required) {
    if (NULL == p) return NULL;
    p->name = name;
    p->desc = desc;
    p->is_required = is_required;
    return p;
}
