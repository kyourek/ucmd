#include <float.h>
#include "ucmdtests.h"

static ucCmd *cmd;

uc_TEST(prior)
    cmd = ucCmd_create();
uc_PASS

uc_TEST(after)
    ucCmd_destroy(cmd);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucArgOpt_is_numeric_returns_is_numeric)
    ucArgOpt o;
    
    o.is_numeric = ucBool_false;
    uc_TRUE(ucBool_false == ucArgOpt_is_numeric(&o));

    o.is_numeric = ucBool_true;
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(&o));
uc_PASS

uc_TEST(ucArgOpt_get_numeric_min_returns_value)
    ucArgOpt o;
    o.numeric_min = -765.432;
    uc_TRUE(-765.432 == ucArgOpt_get_numeric_min(&o));
uc_PASS

uc_TEST(ucArgOpt_get_numeric_max_returns_value)
    ucArgOpt o;
    o.numeric_max = 0.123456789;
    uc_TRUE(0.123456789 == ucArgOpt_get_numeric_max(&o));
uc_PASS

uc_TEST(ucArgOpt_create_creates_arg_opt)
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create("a-name", "a-desc", &a);
    
    uc_TRUE(ucOpt_get_name((ucOpt*)ptr));
    uc_TRUE(ucOpt_get_desc((ucOpt*)ptr));
    uc_TRUE(&a == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    uc_TRUE(0 == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_multiple_creates_arg_opt)
    ucArgOpt *ptr;
    int min_tok_count = 4, max_tok_count = 37;
    const char *name = "a-name", *desc = "a-desc";

    ptr = ucArgOpt_create_multiple(name, desc, min_tok_count, max_tok_count);
    
    uc_TRUE(0 == strcmp(name, ucOpt_get_name((ucOpt*)ptr)));
    uc_TRUE(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    uc_TRUE(NULL == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    uc_TRUE(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(max_tok_count == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple(name, desc, 0, max_tok_count);

    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_required_creates_arg_opt)
    ucArgOpt *ptr;
    ucArgOpt a;

    ptr = ucArgOpt_create_required("aname", "adesc", &a);

    uc_TRUE(ucOpt_get_name((ucOpt*)ptr));
    uc_TRUE(ucOpt_get_desc((ucOpt*)ptr));
    uc_TRUE(&a == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_false == ucArgOpt_is_numeric(ptr));
    uc_TRUE(1 == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_multiple_numeric_creates_arg_opt)
    ucArgOpt *ptr;
    int min_tok_count = 65, max_tok_count = 107;
    double numeric_min = -203.41, numeric_max = +419.26;
    const char *desc = "my_desc";

    ptr = ucArgOpt_create_multiple_numeric(desc, min_tok_count, max_tok_count, numeric_min, numeric_max);
    
    uc_TRUE(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    uc_TRUE(NULL == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    uc_TRUE(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(max_tok_count == ucArgOpt_get_max_tok_count(ptr));
    uc_TRUE(numeric_min == ucArgOpt_get_numeric_min(ptr));
    uc_TRUE(numeric_max == ucArgOpt_get_numeric_max(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple_numeric(desc, 0, max_tok_count, numeric_min, numeric_max);

    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_numeric_creates_arg_opt)
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_numeric("ds", -5.678, 12.34, &a);
    
    uc_TRUE(ucOpt_get_name((ucOpt*)ptr));
    uc_TRUE(ucOpt_get_desc((ucOpt*)ptr));
    uc_TRUE(&a == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    uc_TRUE(-5.678 == ucArgOpt_get_numeric_min(ptr));
    uc_TRUE(12.34 == ucArgOpt_get_numeric_max(ptr));
    uc_TRUE(0 == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_required_numeric_creates_arg_opt)
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    uc_TRUE(ucOpt_get_name((ucOpt*)ptr));
    uc_TRUE(ucOpt_get_desc((ucOpt*)ptr));
    uc_TRUE(&a == ucArgOpt_get_next(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(ptr));
    uc_TRUE(100.436 == ucArgOpt_get_numeric_min(ptr));
    uc_TRUE(567.890 == ucArgOpt_get_numeric_max(ptr));
    uc_TRUE(1 == ucArgOpt_get_min_tok_count(ptr));
    uc_TRUE(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);
uc_PASS

uc_TEST(ucArgOpt_create_creates_different_instances)
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);
    o3 = ucArgOpt_create("o3", "o3", NULL);

    uc_TRUE(o1 != o2);
    uc_TRUE(o2 != o3);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o2);
    ucArgOpt_destroy(o3);
uc_PASS

uc_TEST(ucArgOpt_destroy_releases_instance)
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);

    ucArgOpt_destroy(o2);

    o3 = ucArgOpt_create("o3", "o3", NULL);

    uc_TRUE(o3 == o2);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o3);
uc_PASS

uc_TEST(ucArgOpt_destroy_chain_releases_all_instances)
    ucArgOpt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = ucArgOpt_create("o3", "o3", NULL);
    o2 = ucArgOpt_create("o2", "o2", o3);
    o1 = ucArgOpt_create("o1", "o1", o2);

    ucArgOpt_destroy_chain(o1);

    o3_2 = ucArgOpt_create("o3_2", "o3_2", NULL);
    o2_2 = ucArgOpt_create("o2_2", "o2_2", NULL);
    o1_2 = ucArgOpt_create("o1_2", "o1_2", NULL);

    uc_TRUE(o3 == o3_2);
    uc_TRUE(o2 == o2_2);
    uc_TRUE(o1 == o1_2);

    ucArgOpt_destroy(o3_2);
    ucArgOpt_destroy(o2_2);
    ucArgOpt_destroy(o1_2);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_numeric_err)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "non-num", NULL);
    uc_TRUE(NULL != err);
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument 'non-num' is not numeric."));

    err = ucArgOpt_format_validation_err(a, cmd, "-2.243", NULL);
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_numeric_err_of_switch)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "non-num", "-switch");
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument 'non-num' of '-switch' is not numeric."));

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_out_of_range_numeric)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.5", NULL);
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument '-5.500000' is below the minimum '-5.400000'."));

    err = ucArgOpt_format_validation_err(a, cmd, "2.2", NULL);
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument '2.200000' is above the maximum '2.100000'."));

    err = ucArgOpt_format_validation_err(a, cmd, "-5.399", NULL);
    uc_TRUE(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.099", NULL);
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_out_of_range_numeric_for_switch)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.5", "-mys");
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument '-5.500000' of '-mys' is below the minimum '-5.400000'."));

    err = ucArgOpt_format_validation_err(a, cmd, "2.2", "-thes");
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument '2.200000' of '-thes' is above the maximum '2.100000'."));

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_required_arg)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_required("arg", NULL, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, NULL, NULL);
    uc_TRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "bla", NULL);
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_not_enough_tokens)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    uc_TRUE(NULL != err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_too_many_tokens)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0arg4\0arg5\0arg6\0\n", NULL);
    uc_TRUE(NULL != err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_correct_number_of_tokens)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 3, 3);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_multiple_numbers)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_multiple_numeric(NULL, 2, 8, -100, +100);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0" "4.789" "\0" "notnum" "\0" "91.23" "\0\n", NULL);
    uc_TRUE(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "0.987" "\0" "91.23" "\0\n", NULL);
    uc_TRUE(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "987" "\0" "91.23" "\0\n", NULL);
    uc_TRUE(NULL != err);

    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_create_boolean_creates_boolean_option)
    ucArgOpt *a2 = ucArgOpt_create_boolean("An optional arg", NULL);
    ucArgOpt *a1 = ucArgOpt_create_required_boolean("A required arg", a2);
    uc_TRUE(a2 = ucArgOpt_get_next(a1));
    uc_TRUE(0 == strcmp("<boolean>", ucOpt_get_name((ucOpt*)a2)));
    uc_TRUE(0 == strcmp("<boolean>", ucOpt_get_name((ucOpt*)a1)));
    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)a2));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)a1));
    uc_TRUE(ucBool_true == ucArgOpt_is_boolean(a2));
    uc_TRUE(ucBool_true == ucArgOpt_is_boolean(a1));
    ucArgOpt_destroy(a2);
    ucArgOpt_destroy(a1);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_boolean_errors)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_boolean("b", NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0\n", NULL);
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument '3' is not boolean."));

    err = ucArgOpt_format_validation_err(a, cmd, "invalid" "\0\n", "-bs");
    uc_TRUE(uc_STR_EQ(err, ucOpt_INVALID "Argument 'invalid' of '-bs' is not boolean."));

    ucArgOpt_destroy(a);
uc_PASS

#define ucArgOpt_format_validation_err_ALLOWS_BOOLEAN(VALUE)                \
    ucArgOpt *a = ucArgOpt_create_boolean("b", NULL);                       \
    uc_TRUE(!ucArgOpt_format_validation_err(a, cmd, VALUE "\0\n", NULL));   \
    uc_TRUE(!ucArgOpt_format_validation_err(a, cmd, VALUE "\0\n", NULL));   \
    ucArgOpt_destroy(a);                                                    \

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_1)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("1");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_0)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("0");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_yes)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("yes");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_no)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("no");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_true)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("true");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_false)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("false");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_on)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("on");
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_boolean_off)
    ucArgOpt_format_validation_err_ALLOWS_BOOLEAN("off");
uc_PASS

uc_TEST(ucArgOpt_create_integer_creates_option)
    ucArgOpt *a3 = ucArgOpt_create_multiple_integer("Multiple options", 1, 5, -2500, 1500);
    ucArgOpt *a2 = ucArgOpt_create_integer("An optional arg", 0, 50, a3);
    ucArgOpt *a1 = ucArgOpt_create_required_integer("A required arg", -100, 100, a2);
    uc_TRUE(NULL == ucArgOpt_get_next(a3));
    uc_TRUE(a3 == ucArgOpt_get_next(a2));
    uc_TRUE(a2 = ucArgOpt_get_next(a1));
    uc_TRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a3)));
    uc_TRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a2)));
    uc_TRUE(0 == strcmp("<integer>", ucOpt_get_name((ucOpt*)a1)));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)a3));
    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)a2));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)a1));
    uc_TRUE(ucBool_true == ucArgOpt_is_integer(a3));
    uc_TRUE(ucBool_true == ucArgOpt_is_integer(a2));
    uc_TRUE(ucBool_true == ucArgOpt_is_integer(a1));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(a3));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(a2));
    uc_TRUE(ucBool_true == ucArgOpt_is_numeric(a1));
    uc_TRUE(ucBool_false == ucArgOpt_is_boolean(a3));
    uc_TRUE(ucBool_false == ucArgOpt_is_boolean(a2));
    uc_TRUE(ucBool_false == ucArgOpt_is_boolean(a1));
    uc_TRUE(-2500 == ucArgOpt_get_numeric_min(a3));
    uc_TRUE(1500 == ucArgOpt_get_numeric_max(a3));
    uc_TRUE(0 == ucArgOpt_get_numeric_min(a2));
    uc_TRUE(50 == ucArgOpt_get_numeric_max(a2));
    uc_TRUE(-100 == ucArgOpt_get_numeric_min(a1));
    uc_TRUE(100 == ucArgOpt_get_numeric_max(a1));
    uc_TRUE(0 == strcmp("Multiple options", ucOpt_get_desc((ucOpt*)a3)));
    uc_TRUE(0 == strcmp("An optional arg", ucOpt_get_desc((ucOpt*)a2)));
    uc_TRUE(0 == strcmp("A required arg", ucOpt_get_desc((ucOpt*)a1)));
    ucArgOpt_destroy(a3);
    ucArgOpt_destroy(a2);
    ucArgOpt_destroy(a1);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_catches_integer_errors)
    ucArgOpt *a = ucArgOpt_create_integer("Enter an int", -5, 10, NULL);
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument '3.1' is not an integer.", ucArgOpt_format_validation_err(a, cmd, "3.1" "\0\n", NULL)));
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument 'invalid' of '-ns1' is not an integer.", ucArgOpt_format_validation_err(a, cmd, "invalid" "\0\n", "-ns1")));
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument '-6.000000' is below the minimum '-5.000000'.", ucArgOpt_format_validation_err(a, cmd, "-6" "\0\n", NULL)));
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument '11.000000' of '-ns' is above the maximum '10.000000'.", ucArgOpt_format_validation_err(a, cmd, "11" "\0\n", "-ns")));
    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_allows_valid_integers)
    ucArgOpt *a = ucArgOpt_create_integer("Enter an int", -5, 10, NULL);
    uc_TRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "-5" "\0\n", NULL));
    uc_TRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "-1" "\0\n", NULL));
    uc_TRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "0" "\0\n", NULL));
    uc_TRUE(NULL == ucArgOpt_format_validation_err(a, cmd, "10" "\0\n", NULL));
    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_creates_correct_message_with_too_many_instances)
    ucArgOpt *a = ucArgOpt_create_multiple("mult", NULL, 1, 3);
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument 'mult' has too many instances.", ucArgOpt_format_validation_err(a, cmd, "a1\0a2\0a3\0a4\0\n", NULL)));
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument 'mult' of '-smult' has too many instances.", ucArgOpt_format_validation_err(a, cmd, "a1\0a2\0a3\0a4\0\n", "-smult")));
    ucArgOpt_destroy(a);
uc_PASS

uc_TEST(ucArgOpt_format_validation_err_creates_correct_message_with_too_few_instances)
    ucArgOpt *a = ucArgOpt_create_multiple("mult", NULL, 5, 6);
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument 'mult' has too few instances.", ucArgOpt_format_validation_err(a, cmd, "a1\0a2\0a3\0a4\0\n", NULL)));
    uc_TRUE(uc_STR_EQ(ucOpt_INVALID "Argument 'mult' of '-smult' has too few instances.", ucArgOpt_format_validation_err(a, cmd, "a1\0a2\0a3\0a4\0\n", "-smult")));
    ucArgOpt_destroy(a);
uc_PASS

uc_TEST_GROUP(ucArgOpt, setup,
    ucArgOpt_create_creates_arg_opt,
    ucArgOpt_create_required_creates_arg_opt,
    ucArgOpt_create_numeric_creates_arg_opt,
    ucArgOpt_create_required_numeric_creates_arg_opt,
    ucArgOpt_create_creates_different_instances,
    ucArgOpt_create_multiple_creates_arg_opt,
    ucArgOpt_create_multiple_numeric_creates_arg_opt,
    ucArgOpt_create_boolean_creates_boolean_option,
    ucArgOpt_create_integer_creates_option,
    ucArgOpt_destroy_releases_instance,
    ucArgOpt_destroy_chain_releases_all_instances,
    ucArgOpt_format_validation_err_allows_correct_number_of_tokens,
    ucArgOpt_format_validation_err_allows_boolean_1,
    ucArgOpt_format_validation_err_allows_boolean_0,
    ucArgOpt_format_validation_err_allows_boolean_on,
    ucArgOpt_format_validation_err_allows_boolean_off,
    ucArgOpt_format_validation_err_allows_boolean_yes,
    ucArgOpt_format_validation_err_allows_boolean_no,
    ucArgOpt_format_validation_err_allows_boolean_true,
    ucArgOpt_format_validation_err_allows_boolean_false,
    ucArgOpt_format_validation_err_allows_valid_integers,
    ucArgOpt_format_validation_err_catches_numeric_err,
    ucArgOpt_format_validation_err_catches_numeric_err_of_switch,
    ucArgOpt_format_validation_err_catches_out_of_range_numeric,
    ucArgOpt_format_validation_err_catches_out_of_range_numeric_for_switch,
    ucArgOpt_format_validation_err_catches_required_arg,
    ucArgOpt_format_validation_err_catches_not_enough_tokens,
    ucArgOpt_format_validation_err_catches_too_many_tokens,
    ucArgOpt_format_validation_err_catches_integer_errors,
    ucArgOpt_format_validation_err_catches_multiple_numbers,
    ucArgOpt_format_validation_err_catches_boolean_errors,
    ucArgOpt_format_validation_err_creates_correct_message_with_too_few_instances,
    ucArgOpt_format_validation_err_creates_correct_message_with_too_many_instances,
    ucArgOpt_get_numeric_min_returns_value,
    ucArgOpt_get_numeric_max_returns_value,
    ucArgOpt_is_numeric_returns_is_numeric)
