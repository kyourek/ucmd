#ifndef CL_TESTS_P_H
#define CL_TESTS_P_H

#include "cl_tests.h"

struct cl_tests {
    int run_count;
    int run_group_count;
    int assertions_made;
    int group_run_count;
    int group_assertions_made;
    cl_bool unattended;
    cl_tests_err err_returned;
    
    int run_test_count;
    int run_group_count;
    int run_group_test_count;

    cl_tests_print_func *print_func;
    void *print_state;
    
    cl_tests_exit_func *exit_func;
    void *exit_state;
};

extern cl_tests *cl_tests_running_instance;

#define CL_TESTS_FAIL() \
    do { \
        return -1; \
    } while (0) \

#define CL_TESTS_ASSERT(assertion) \
    do { \
        if (!(assertion)) { \
            return -1; \
        } \
        cl_tests_running_instance->assertions_made++; \
        cl_tests_running_instance->group_assertions_made++; \
    } while (0) \

#define CL_TESTS_RUN(test) \
    do { \
        if ((test())) { \
            return -1; \
        } \
        cl_tests_running_instance->run_count++; \
        cl_tests_running_instance->group_run_count++; \
    } while (0) \

#define CL_TESTS_RUN_GROUP(test_group) \
    do { \
        cl_tests_running_instance->group_run_count = 0; \
        cl_tests_running_instance->group_assertions_made = 0; \
        if ((test_group())) { \
            return -1; \
        } \
        cl_tests_running_instance->run_group_count++; \
    } while (0) \

#endif
