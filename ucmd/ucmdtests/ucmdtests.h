/********************************************************************************
*  Copyright (c) 2013-2015 Ken Yourek
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*******************************************************************************/

#ifndef     UCMDTESTS_H
#define     UCMDTESTS_H

#include    <stdio.h>

#include    "ucmd.h"
#include    "ucmd_internal.h"

#ifndef     ucTestGroup_COUNT
#define     ucTestGroup_COUNT                           20
#endif

#define     ucFAIL()                                    do { return -1; } while (0)
#define     ucPASS()                                    do { return -0; } while (0)
#define     ucTRUE(TRUE)                                do { if (!(TRUE)) { printf("%s\n", #TRUE); return -1; } } while (0)
#define     uc_TEST(NAME)                               static int NAME(ucTestGroup *p) {
#define     uc_PASS                                     return 0; }
#define     uc_TRUE(EXPRESSION)                         do { if (!(EXPRESSION)) { return -1; } } while (0)
#define     uc_TEST_GROUP_TEST(NAME)                    { #NAME, NAME }
#define     uc_TEST_GROUP(NAME, SETUP, ...)             \
    static ucTestGroup *uc_test_group(void) {           \
        static ucTestGroup test_group;                  \
        static ucTestGroup_TestFunc *test[] = {         \
            __VA_ARGS__,                                \
            NULL };                                     \
        ucTestGroup_init(&test_group);                  \
        ucTestGroup_set_test(&test_group, test);        \
        ucTestGroup_set_name(&test_group, #NAME);       \
        ucTestGroup_set_setup(&test_group, SETUP);      \
        return &test_group;                             \
    }                                                   \
    ucTestGroup* ucTestGroup_ ## NAME(void) {           \
        return uc_test_group();                         \
    }                                                   \

#define     uc_TEST_RUNNER(NAME, ...)                   \
    static ucTestRunner uc_test_runner(void) {          \
        static ucTestRunner test_runner;                \
        static ucTestGroup *group[] = {                 \
            __VA_ARGS__,                                \
            NULL };                                     \
        return ucTestRunner_init                        \
            &test_runner,                               \
            #NAME,                                      \
            group);                                     \
    }                                                   \
    ucTestRunner* ucTestRunner_ ## NAME(void) {         \
        return uc_test_runner();                        \
    }                                                   \

typedef     struct ucTest                               ucTest;
typedef     int                                         ucTestErr;
typedef     struct ucTestGroup                          ucTestGroup;
typedef     struct ucTestGroup_Test                     ucTestGroup_Test;
typedef     struct ucTestRunner                         ucTestRunner;
typedef     struct ucTestState                          ucTestState;

uc_EXPORTED int                                         ucTestState_get_run_group_count(ucTestState*);
uc_EXPORTED int                                         ucTestState_get_run_group_test_count(ucTestState*);
uc_EXPORTED int                                         ucTestState_get_run_test_count(ucTestState*);
uc_EXPORTED ucTestState*                                ucTestState_init(ucTestState*);
uc_EXPORTED void                                        ucTestState_reset(ucTestState*);
uc_EXPORTED void                                        ucTestState_set_run_group_count(ucTestState*, int value);
uc_EXPORTED void                                        ucTestState_set_run_group_test_count(ucTestState*, int value);
uc_EXPORTED void                                        ucTestState_set_run_test_count(ucTestState*, int value);
            struct                                      ucTestState {
            int                                         run_test_count;
            int                                         run_group_count;
            int                                         run_group_test_count; };

typedef     int                                         (ucTestGroup_TestFunc)(ucTestGroup*);
uc_EXPORTED ucTestGroup*                                ucTestGroup_init(ucTestGroup*);
uc_EXPORTED const char*                                 ucTestGroup_get_name(ucTestGroup*);
uc_EXPORTED int                                         ucTestGroup_run(ucTestGroup*, ucTestState *state);
uc_EXPORTED void                                        ucTestGroup_set_name(ucTestGroup*, const char*);
uc_EXPORTED void                                        ucTestGroup_set_setup(ucTestGroup*, ucTestGroup_TestFunc*);
uc_EXPORTED void                                        ucTestGroup_set_test(ucTestGroup*, ucTestGroup_TestFunc**);
uc_EXPORTED void                                        ucTestGroup_setup_test(ucTestGroup*, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after);
            struct                                      ucTestGroup {
            const char*                                 name;
            ucTestGroup_TestFunc**                      test;
            ucTestGroup_TestFunc*                       setup;
            ucTestGroup_TestFunc*                       prior;
            ucTestGroup_TestFunc*                       after; };

            struct                                      ucTestGroup_Test {
            const char*                                 name;
            ucTestGroup_TestFunc*                       work; };

typedef     void                                        (ucTest_PrintFunc)(const char *str, void *state);
typedef     ucBool                                      (ucTest_ExitFunc)(void* state);
uc_EXPORTED ucTestGroup**                               ucTest_get_groups(ucTest*);
uc_EXPORTED const char*                                 ucTest_get_label(ucTest*);
uc_EXPORTED ucTestState*                                ucTest_get_state(ucTest*);
uc_EXPORTED ucTest*                                     ucTest_init(ucTest*, const char *label, ucTestGroup **groups);
uc_EXPORTED ucTestErr                                   ucTest_run(ucTest*);
uc_EXPORTED void                                        ucTest_set_exit_func(ucTest*, ucTest_ExitFunc *value);
uc_EXPORTED void                                        ucTest_set_exit_state(ucTest*, void *value);
uc_EXPORTED void                                        ucTest_set_print_func(ucTest*, ucTest_PrintFunc *value);
uc_EXPORTED void                                        ucTest_set_print_state(ucTest*, void *value);
            struct                                      ucTest {
            const char*                                 label;
            ucTestState                                 state;
            ucTest_PrintFunc*                           print_func;
            void*                                       print_state;
            ucTest_ExitFunc*                            exit_func;
            void*                                       exit_state;
            ucTestGroup**                               groups; };

uc_EXPORTED ucTestRunner*                               ucTestRunner_init(ucTestRunner*, const char *name, ucTestGroup **group);
            struct                                      ucTestRunner {
            const char*                                 name;
            ucTestGroup**                               group; };

uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgOpt(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgOptOwner(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgTok(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgTokOwner(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucBool(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLine(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineApp(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineOpt(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineToks(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdParser(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdTok(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucOpt(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucSwitchOpt(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucSwitchTok(void);
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucTok(void);

uc_EXPORTED ucTest*                                     ucTests_get_test(void);

#endif
