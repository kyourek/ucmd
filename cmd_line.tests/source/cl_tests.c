#include "cl_tests.h"
#include "cl_arg_opt_tests.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_arg_tok_tests.h"
#include "cl_cmd_parser_tests.h"
#include "cl_cmd_tok_tests.h"
#include "cl_common_tests.h"
#include "cl_opt_tests.h"
#include "cl_switch_opt_tests.h"
#include "cl_switch_tok_tests.h"
#include "cl_tok_tests.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_cmd_line_opt_tests.h"
#include "cl_cmd_line_tests.h"

int cl_tests_err_returned = CL_TESTS_NO_ERR;

int cl_tests_run_count = 0;
int cl_tests_run_group_count = 0;
int cl_tests_assertions_made = 0;

int cl_tests_group_run_count = 0;
int cl_tests_group_assertions_made = 0;

cl_tests_err  cl_tests(void) {
    CL_TESTS_RUN_GROUP(cl_arg_opt_tests);
    CL_TESTS_RUN_GROUP(cl_arg_tok_owner_tests);
    CL_TESTS_RUN_GROUP(cl_arg_tok_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_parser_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_tok_tests);
    CL_TESTS_RUN_GROUP(cl_common_tests);
    CL_TESTS_RUN_GROUP(cl_opt_tests);
    CL_TESTS_RUN_GROUP(cl_switch_opt_tests);
    CL_TESTS_RUN_GROUP(cl_switch_tok_tests);
    CL_TESTS_RUN_GROUP(cl_tok_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_app_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_opt_tests);
    CL_TESTS_RUN_GROUP(cl_cmd_line_tests);
    return CL_TESTS_NO_ERR;
}
