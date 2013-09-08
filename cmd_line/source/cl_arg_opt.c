#include <stdlib.h>
#include "cl_common.h"
#include "cl_memory_manager_p.h"
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"

CL_MEMORY_MANAGER_INIT(cl_arg_opt, (CL_CMD_COUNT_MAX * CL_ARG_COUNT_MAX) + (CL_SWITCH_COUNT_MAX * CL_ARG_COUNT_MAX));

static cl_arg_opt *create_arg_opt(void) {
    return cl_memory_manager_create();
}

CL_BOOL cl_arg_opt_is_numeric(cl_arg_opt *p) {
    if (NULL == p) return CL_FALSE;
    return p->is_numeric;
}

double cl_arg_opt_get_numeric_min(cl_arg_opt *p) {
    if (NULL == p) return 0;
    return p->numeric_min;
}

double cl_arg_opt_get_numeric_max(cl_arg_opt* p) {
    if (NULL == p) return 0;
    return p->numeric_max;
}

cl_arg_opt *cl_arg_opt_get_next(cl_arg_opt* p) {
    if (NULL == p) return NULL;
    return p->next;
}

cl_arg_opt *cl_arg_opt_init(cl_arg_opt *p, const char *name, const char *desc, CL_BOOL is_required, CL_BOOL is_numeric, double numeric_min, double numeric_max, cl_arg_opt *next) {

    if (NULL == p) return NULL;
    if (NULL == cl_opt_init((cl_opt*)p, name, desc, is_required)) return NULL;

    p->is_numeric = is_numeric;
    p->numeric_min = numeric_min;
    p->numeric_max = numeric_max;
    p->next = next;

    return p;
}

cl_arg_opt *cl_arg_opt_create(const char *name, const char *desc, cl_arg_opt *next) {
    return cl_arg_opt_init(create_arg_opt(), name, desc, CL_FALSE, CL_FALSE, 0, 0, next);
}

cl_arg_opt *cl_arg_opt_create_required(const char *name, const char *desc, cl_arg_opt *next) {
    return cl_arg_opt_init(create_arg_opt(), name, desc, CL_TRUE, CL_FALSE, 0, 0, next);
}

cl_arg_opt *cl_arg_opt_create_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next) {
    return cl_arg_opt_init(create_arg_opt(), "<number>", desc, CL_FALSE, CL_TRUE, numeric_min, numeric_max, next);
}

cl_arg_opt *cl_arg_opt_create_required_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next) {
    return cl_arg_opt_init(create_arg_opt(), "<number>", desc, CL_TRUE, CL_TRUE, numeric_min, numeric_max, next);
}

void cl_arg_opt_destroy(cl_arg_opt *p) {
    cl_memory_manager_destroy(p);
}

void cl_arg_opt_destroy_chain(cl_arg_opt *p) {
    cl_arg_opt *next = p;
    while (NULL != next) {
        p = next;
        next = cl_arg_opt_get_next(p);
        cl_arg_opt_destroy(p);
    }
}
