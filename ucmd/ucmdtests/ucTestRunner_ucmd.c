#include "ucmdtests.h"

uc_TEST_RUNNER(ucmd,
    &ucTestGroup_ucArgOpt,
    &ucTestGroup_ucArgOptOwner,
    &ucTestGroup_ucArgTokOwner,
    &ucTestGroup_ucArgTok,
    &ucTestGroup_ucBool,
    &ucTestGroup_ucCmdParser,
    &ucTestGroup_ucCmdTok,
    &ucTestGroup_ucOpt,
    &ucTestGroup_ucSwitchOpt,
    &ucTestGroup_ucSwitchTok,
    &ucTestGroup_ucTok,
    &ucTestGroup_ucCmdLineApp,
    &ucTestGroup_ucCmdLineOpt,
    &ucTestGroup_ucCmdLine,
    &ucTestGroup_ucCmdLineToks)
