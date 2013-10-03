#ifndef CL_TESTS_H
#define CL_TESTS_H

#include "cl_bool.h"
#include "cl_common.h"
        
#define CL_TESTS_ERR int
#define CL_TESTS_NO_ERR 0

typedef void (cl_tests_print_func)(const char *str);

typedef cl_bool (cl_tests_exit_func)(void);

typedef struct cl_tests cl_tests;

CL_EXPORTED cl_tests *cl_tests_get_instance(void);

CL_EXPORTED int cl_tests_get_run_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_run_group_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_assertions_made(cl_tests *p);

CL_EXPORTED int cl_tests_get_group_run_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_group_assertions_made(cl_tests *p);

CL_EXPORTED cl_bool cl_tests_get_unattended(cl_tests *p);

CL_EXPORTED void cl_tests_set_unattended(cl_tests *p, cl_bool value);

CL_EXPORTED CL_TESTS_ERR cl_tests_get_err_returned(cl_tests *p);

CL_EXPORTED cl_tests_print_func *cl_tests_get_print(cl_tests *p);

CL_EXPORTED void cl_tests_set_print(cl_tests *p, cl_tests_print_func *value);

CL_EXPORTED cl_tests_exit_func *cl_tests_get_exit(cl_tests *p);

CL_EXPORTED void cl_tests_set_exit(cl_tests *p, cl_tests_exit_func *value);

CL_EXPORTED CL_TESTS_ERR cl_tests_run(cl_tests *p);

CL_EXPORTED CL_TESTS_ERR cl_tests_main(cl_tests *p);

#endif
