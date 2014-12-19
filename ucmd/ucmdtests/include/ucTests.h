#ifndef UC_TESTS_H
#define UC_TESTS_H

#include "ucTest.h"

UC_EXPORTED ucTestGroup *ucArgOpt_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucArgTokOwner_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucArgTok_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucBool_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdLineApp_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdLineOpt_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdLine_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdLineToks_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdParser_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucCmdTok_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucOpt_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucSwitchOpt_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucSwitchTok_tests_get_group(void);
UC_EXPORTED ucTestGroup *ucTok_tests_get_group(void);

UC_EXPORTED ucTest *ucTests_get_test(void);

#endif
