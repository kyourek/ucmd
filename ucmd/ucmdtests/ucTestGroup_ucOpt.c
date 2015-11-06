#include "ucmdtests.h"

uc_TEST(ucOpt_get_name_returns_name)
    ucOpt o;

    o.name = "name 1";
    uc_TRUE(ucOpt_get_name(&o));

    o.name = "two NAME";
    uc_TRUE(ucOpt_get_name(&o));
uc_PASS

uc_TEST(ucOpt_get_desc_returns_description)
    ucOpt o;

    o.desc = "description one";
    uc_TRUE(ucOpt_get_desc(&o));

    o.desc = "2 desc";
    uc_TRUE(ucOpt_get_desc(&o));
uc_PASS

uc_TEST(ucOpt_init_initializes_structure)
    ucOpt o;
    ucOpt *ptr = ucOpt_init(&o, "my_command_name", "Description of command.", ucBool_true);
    uc_TRUE(ptr == (&o));
    uc_TRUE(ucOpt_get_name(ptr));
    uc_TRUE(ucOpt_get_desc(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required(ptr));
uc_PASS

uc_TEST(ucOpt_is_required_returns_is_required)
    ucOpt o;
    o.is_required = ucBool_true;
    uc_TRUE(ucBool_true == ucOpt_is_required(&o));
    o.is_required = ucBool_false;
    uc_TRUE(ucBool_false == ucOpt_is_required(&o));
uc_PASS

uc_TEST_GROUP(ucOpt, NULL,
    ucOpt_get_name_returns_name,
    ucOpt_get_desc_returns_description,
    ucOpt_init_initializes_structure,
    ucOpt_is_required_returns_is_required)
