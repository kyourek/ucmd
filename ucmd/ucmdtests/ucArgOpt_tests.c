#include <float.h>
#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucArgOpt_is_numeric_returns_is_numeric(ucTestGroup *p) {
    ucArgOpt o;
    
    o.is_numeric = ucBool_false;
    ucTRUE(ucBool_false == ucArgOpt_is_numeric(&o));

    o.is_numeric = ucBool_true;
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(&o));

    ucPASS();
}

static ucTestErr ucArgOpt_get_numeric_min_returns_value(ucTestGroup *p) {
    ucArgOpt o;
    o.numeric_min = -765.432;
    ucTRUE(-765.432 == ucArgOpt_get_numeric_min(&o));
    ucPASS();
}

static ucTestErr ucArgOpt_get_numeric_max_returns_value(ucTestGroup *p) {
    ucArgOpt o;
    o.numeric_max = 0.123456789;
    ucTRUE(0.123456789 == ucArgOpt_get_numeric_max(&o));
    ucPASS();
}

static ucTestErr ucArgOpt_create_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create("a-name", "a-desc", &a);
    
    ucTRUE(ucOpt_get_name((ucOpt*)ptr));
    ucTRUE(ucOpt_get_desc((ucOpt*)ptr));
    ucTRUE(&a == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTRUE(0 == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_multiple_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    int min_tok_count = 4, max_tok_count = 37;
    const char *name = "a-name", *desc = "a-desc";

    ptr = ucArgOpt_create_multiple(name, desc, min_tok_count, max_tok_count);
    
    ucTRUE(0 == strcmp(name, ucOpt_get_name((ucOpt*)ptr)));
    ucTRUE(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    ucTRUE(NULL == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTRUE(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(max_tok_count == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple(name, desc, 0, max_tok_count);

    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_required_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;

    ptr = ucArgOpt_create_required("aname", "adesc", &a);

    ucTRUE(ucOpt_get_name((ucOpt*)ptr));
    ucTRUE(ucOpt_get_desc((ucOpt*)ptr));
    ucTRUE(&a == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTRUE(1 == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_multiple_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    int min_tok_count = 65, max_tok_count = 107;
    double numeric_min = -203.41, numeric_max = +419.26;
    const char *desc = "my_desc";

    ptr = ucArgOpt_create_multiple_numeric(desc, min_tok_count, max_tok_count, numeric_min, numeric_max);
    
    ucTRUE(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    ucTRUE(NULL == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTRUE(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(max_tok_count == ucArgOpt_get_max_tok_count(ptr));
    ucTRUE(numeric_min == ucArgOpt_get_numeric_min(ptr));
    ucTRUE(numeric_max == ucArgOpt_get_numeric_max(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple_numeric(desc, 0, max_tok_count, numeric_min, numeric_max);

    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_numeric("ds", -5.678, 12.34, &a);
    
    ucTRUE(ucOpt_get_name((ucOpt*)ptr));
    ucTRUE(ucOpt_get_desc((ucOpt*)ptr));
    ucTRUE(&a == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTRUE(-5.678 == ucArgOpt_get_numeric_min(ptr));
    ucTRUE(12.34 == ucArgOpt_get_numeric_max(ptr));
    ucTRUE(0 == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_required_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    ucTRUE(ucOpt_get_name((ucOpt*)ptr));
    ucTRUE(ucOpt_get_desc((ucOpt*)ptr));
    ucTRUE(&a == ucArgOpt_get_next(ptr));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTRUE(100.436 == ucArgOpt_get_numeric_min(ptr));
    ucTRUE(567.890 == ucArgOpt_get_numeric_max(ptr));
    ucTRUE(1 == ucArgOpt_get_min_tok_count(ptr));
    ucTRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ucPASS();
}

static ucTestErr ucArgOpt_create_creates_different_instances(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);
    o3 = ucArgOpt_create("o3", "o3", NULL);

    ucTRUE(o1 != o2);
    ucTRUE(o2 != o3);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o2);
    ucArgOpt_destroy(o3);

    ucPASS();
}

static ucTestErr ucArgOpt_destroy_releases_instance(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);

    ucArgOpt_destroy(o2);

    o3 = ucArgOpt_create("o3", "o3", NULL);

    ucTRUE(o3 == o2);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o3);

    ucPASS();
}

static ucTestErr ucArgOpt_destroy_chain_releases_all_instances(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = ucArgOpt_create("o3", "o3", NULL);
    o2 = ucArgOpt_create("o2", "o2", o3);
    o1 = ucArgOpt_create("o1", "o1", o2);

    ucArgOpt_destroy_chain(o1);

    o3_2 = ucArgOpt_create("o3_2", "o3_2", NULL);
    o2_2 = ucArgOpt_create("o2_2", "o2_2", NULL);
    o1_2 = ucArgOpt_create("o1_2", "o1_2", NULL);

    ucTRUE(o3 == o3_2);
    ucTRUE(o2 == o2_2);
    ucTRUE(o1 == o1_2);

    ucArgOpt_destroy(o3_2);
    ucArgOpt_destroy(o2_2);
    ucArgOpt_destroy(o1_2);

    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_numeric_err(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "non-num", NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-2.243", NULL);
    ucTRUE(NULL == err);

    ucArgOpt_destroy(a);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_out_of_range_numeric(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.5", NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.2", NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.399", NULL);
    ucTRUE(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.099", NULL);
    ucTRUE(NULL == err);

    ucArgOpt_destroy(a);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_required_arg(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_required("arg", NULL, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, NULL, NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "bla", NULL);
    ucTRUE(NULL == err);

    ucArgOpt_destroy(a);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_not_enough_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    ucTRUE(NULL != err);

    ucArgOpt_destroy(a);

    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_too_many_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0arg4\0arg5\0arg6\0\n", NULL);
    ucTRUE(NULL != err);

    ucArgOpt_destroy(a);

    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_allows_correct_number_of_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 3, 3);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    ucTRUE(NULL == err);

    ucArgOpt_destroy(a);

    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err_catches_multiple_numbers(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_multiple_numeric(NULL, 2, 8, -100, +100);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0" "4.789" "\0" "notnum" "\0" "91.23" "\0\n", NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "0.987" "\0" "91.23" "\0\n", NULL);
    ucTRUE(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "987" "\0" "91.23" "\0\n", NULL);
    ucTRUE(NULL != err);

    ucArgOpt_destroy(a);

    ucPASS();
}

static ucTestErr ucArgOpt_create_boolean__creates_boolean_option(ucTestGroup *p) {
    ucArgOpt *a2 = ucArgOpt_create_boolean("An optional arg", NULL);
    ucArgOpt *a1 = ucArgOpt_create_required_boolean("A required arg", a2);
    ucTRUE(a2 = ucArgOpt_get_next(a1));
    ucTRUE(0 == strcmp("<boolean>", ucOpt_get_name((ucOpt*)a2)));
    ucTRUE(0 == strcmp("<boolean>", ucOpt_get_name((ucOpt*)a1)));
    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)a2));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)a1));
    ucTRUE(ucBool_true == ucArgOpt_is_boolean(a2));
    ucTRUE(ucBool_true == ucArgOpt_is_boolean(a1));
    ucArgOpt_destroy(a2);
    ucArgOpt_destroy(a1);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err__catches_boolean_errors(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_boolean("b", NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0\n", NULL);
    ucTRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "invalid" "\0\n", NULL);
    ucTRUE(NULL != err);

    ucArgOpt_destroy(a);

    ucPASS();
}

#define ucArgOpt_format_validation_err__ALLOWS_BOOLEAN(VALUE)                   \
    const char *err;                                                            \
    ucCmdLine *cmd = ucCmdLine_instance();                                  \
    ucArgOpt *a = ucArgOpt_create_boolean("b", NULL);                           \
    err = ucArgOpt_format_validation_err(a, cmd, VALUE "\0\n", NULL);           \
    ucTRUE(NULL == err);                                                 \
    err = ucArgOpt_format_validation_err(a, cmd, VALUE "\0\n", NULL);           \
    ucTRUE(NULL == err);                                                 \
    ucArgOpt_destroy(a);                                                        \
    ucPASS();                                                          

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_1(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("1");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_0(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("0");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_yes(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("yes");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_no(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("no");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_true(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("true");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_false(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("false");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_on(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("on");
}

static ucTestErr ucArgOpt_format_validation_err__allows_boolean_off(ucTestGroup *p) {
    ucArgOpt_format_validation_err__ALLOWS_BOOLEAN("off");
}

static ucTestErr ucArgOpt_create_integer__creates_option(ucTestGroup *p) {
    ucArgOpt *a3 = ucArgOpt_create_multiple_integer("Multiple options", 1, 5, -2500, 1500);
    ucArgOpt *a2 = ucArgOpt_create_integer("An optional arg", 0, 50, a3);
    ucArgOpt *a1 = ucArgOpt_create_required_integer("A required arg", -100, 100, a2);
    ucTRUE(NULL == ucArgOpt_get_next(a3));
    ucTRUE(a3 == ucArgOpt_get_next(a2));
    ucTRUE(a2 = ucArgOpt_get_next(a1));
    ucTRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a3)));
    ucTRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a2)));
    ucTRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a1)));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)a3));
    ucTRUE(ucBool_false == ucOpt_is_required((ucOpt*)a2));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)a1));
    ucTRUE(ucBool_true == ucArgOpt_is_integer(a3));
    ucTRUE(ucBool_true == ucArgOpt_is_integer(a2));
    ucTRUE(ucBool_true == ucArgOpt_is_integer(a1));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(a3));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(a2));
    ucTRUE(ucBool_true == ucArgOpt_is_numeric(a1));
    ucTRUE(ucBool_false == ucArgOpt_is_boolean(a3));
    ucTRUE(ucBool_false == ucArgOpt_is_boolean(a2));
    ucTRUE(ucBool_false == ucArgOpt_is_boolean(a1));
    ucTRUE(-2500 == ucArgOpt_get_numeric_min(a3));
    ucTRUE(1500 == ucArgOpt_get_numeric_max(a3));
    ucTRUE(0 == ucArgOpt_get_numeric_min(a2));
    ucTRUE(50 == ucArgOpt_get_numeric_max(a2));
    ucTRUE(-100 == ucArgOpt_get_numeric_min(a1));
    ucTRUE(100 == ucArgOpt_get_numeric_max(a1));
    ucTRUE(0 == strcmp("Multiple options", ucOpt_get_desc((ucOpt*)a3)));
    ucTRUE(0 == strcmp("An optional arg", ucOpt_get_desc((ucOpt*)a2)));
    ucTRUE(0 == strcmp("A required arg", ucOpt_get_desc((ucOpt*)a1)));
    ucArgOpt_destroy(a3);
    ucArgOpt_destroy(a2);
    ucArgOpt_destroy(a1);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err__catches_integer_errors(ucTestGroup *p) {
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_integer("Enter an int", -5, 10, NULL);
    ucTRUE(NULL != ucArgOpt_format_validation_err(a, cmd, "3.1" "\0\n", NULL));
    ucTRUE(NULL != ucArgOpt_format_validation_err(a, cmd, "invalid" "\0\n", NULL));
    ucTRUE(NULL != ucArgOpt_format_validation_err(a, cmd, "-6" "\0\n", NULL));
    ucTRUE(NULL != ucArgOpt_format_validation_err(a, cmd, "11" "\0\n", NULL));
    ucArgOpt_destroy(a);
    ucPASS();
}

static ucTestErr ucArgOpt_format_validation_err__allows_valid_integers(ucTestGroup *p) {
    ucCmdLine *cmd = ucCmdLine_instance();
    ucArgOpt *a = ucArgOpt_create_integer("Enter an int", -5, 10, NULL);
    ucTRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "-5" "\0\n", NULL));
    ucTRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "-1" "\0\n", NULL));
    ucTRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "0" "\0\n", NULL));
    ucTRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "10" "\0\n", NULL));
    ucArgOpt_destroy(a);
    ucPASS();
}

ucTestGroup *ucArgOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucArgOpt_is_numeric_returns_is_numeric,
        ucArgOpt_get_numeric_min_returns_value,
        ucArgOpt_get_numeric_max_returns_value,
        ucArgOpt_create_creates_arg_opt,
        ucArgOpt_create_required_creates_arg_opt,
        ucArgOpt_create_numeric_creates_arg_opt,
        ucArgOpt_create_required_numeric_creates_arg_opt,
        ucArgOpt_create_creates_different_instances,
        ucArgOpt_destroy_releases_instance,
        ucArgOpt_destroy_chain_releases_all_instances,
        ucArgOpt_format_validation_err_catches_numeric_err,
        ucArgOpt_format_validation_err_catches_out_of_range_numeric,
        ucArgOpt_format_validation_err_catches_required_arg,
        ucArgOpt_create_multiple_creates_arg_opt,
        ucArgOpt_create_multiple_numeric_creates_arg_opt,
        ucArgOpt_format_validation_err_catches_not_enough_tokens,
        ucArgOpt_format_validation_err_catches_too_many_tokens,
        ucArgOpt_format_validation_err_allows_correct_number_of_tokens,
        ucArgOpt_format_validation_err_catches_multiple_numbers,
        ucArgOpt_create_boolean__creates_boolean_option,
        ucArgOpt_format_validation_err__catches_boolean_errors,
        ucArgOpt_format_validation_err__allows_boolean_1,
        ucArgOpt_format_validation_err__allows_boolean_0,
        ucArgOpt_format_validation_err__allows_boolean_on,
        ucArgOpt_format_validation_err__allows_boolean_off,
        ucArgOpt_format_validation_err__allows_boolean_yes,
        ucArgOpt_format_validation_err__allows_boolean_no,
        ucArgOpt_format_validation_err__allows_boolean_true,
        ucArgOpt_format_validation_err__allows_boolean_false,
        ucArgOpt_create_integer__creates_option,
        ucArgOpt_format_validation_err__catches_integer_errors,
        ucArgOpt_format_validation_err__allows_valid_integers,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
