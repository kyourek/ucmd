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

int cl_tests_run = 0;
int cl_assertions = 0;

char *cl_run_all_tests(void) {
    cl_run_test(cl_arg_opt_tests);
    cl_run_test(cl_arg_tok_owner_tests);
    cl_run_test(cl_arg_tok_tests);
    cl_run_test(cl_cmd_parser_tests);
    cl_run_test(cl_cmd_tok_tests);
    cl_run_test(cl_common_tests);
    cl_run_test(cl_opt_tests);
    cl_run_test(cl_switch_opt_tests);
    cl_run_test(cl_switch_tok_tests);
    cl_run_test(cl_tok_tests);
    cl_run_test(cl_cmd_line_app_tests);
    cl_run_test(cl_cmd_line_opt_tests);
    cl_run_test(cl_cmd_line_tests);
    return 0;
}
