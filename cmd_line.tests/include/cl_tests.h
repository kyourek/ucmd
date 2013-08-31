#ifndef CL_TESTS_H
#define CL_TESTS_H

#include "cl_common.h"

#define CL_TESTS_FAIL() \
    do { \
        return -1; \
    } while (0) \

#define CL_TESTS_RUN(test) \
    do { \
        cl_tests_err_returned = test(); \
        if (cl_tests_err_returned) { \
            return cl_tests_err_returned; \
        } \
        cl_tests_run_count++; \
        cl_tests_group_run_count++; \
    } while (0) \

#define CL_TESTS_ASSERT(test) \
    do { \
        if (!(test)) { \
            return -1; \
        } \
        cl_tests_assertions_made++; \
        cl_tests_group_assertions_made++; \
    } while (0) \

#define CL_TESTS_RUN_GROUP(group_test) \
    do { \
        cl_tests_group_run_count = 0; \
        cl_tests_group_assertions_made = 0; \
        cl_tests_err_returned = group_test(); \
        if (cl_tests_err_returned) { \
            return cl_tests_err_returned; \
        } \
        cl_tests_run_group_count++; \
    } while (0) \
        
#define CL_TESTS_NO_ERR 0;

typedef int cl_tests_err;

extern cl_tests_err cl_tests_err_returned;

extern int cl_tests_run_count;
extern int cl_tests_run_group_count;
extern int cl_tests_assertions_made;

extern int cl_tests_group_run_count;
extern int cl_tests_group_assertions_made;

CL_EXPORTED cl_tests_err cl_tests(void);

#endif
