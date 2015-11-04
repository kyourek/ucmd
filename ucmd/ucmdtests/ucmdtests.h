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

#define     ucFAIL()                            do { return -1; } while (0)
#define     ucPASS()                            do { return -0; } while (0)
#define     ucTRUE(TRUE)                        do { if (!(TRUE)) { printf("%s\n", #TRUE); return -1; } } while (0)

typedef     struct ucTest                       ucTest;
typedef     int                                 ucTestErr;
typedef     struct ucTestGroup                  ucTestGroup;
typedef     struct ucTestState                  ucTestState;

uc_EXPORTED int                                 ucTestState_get_run_group_count(ucTestState*);
uc_EXPORTED int                                 ucTestState_get_run_group_test_count(ucTestState*);
uc_EXPORTED int                                 ucTestState_get_run_test_count(ucTestState*);
uc_EXPORTED ucTestState*                        ucTestState_init(ucTestState*);
uc_EXPORTED void                                ucTestState_reset(ucTestState*);
uc_EXPORTED void                                ucTestState_set_run_group_count(ucTestState*, int value);
uc_EXPORTED void                                ucTestState_set_run_group_test_count(ucTestState*, int value);
uc_EXPORTED void                                ucTestState_set_run_test_count(ucTestState*, int value);
            struct                              ucTestState {
            int                                 run_test_count;
            int                                 run_group_count;
            int                                 run_group_test_count; };
            
typedef     ucTestErr                           (ucTestGroup_CallbackFunc)(ucTestGroup*);
typedef     ucTestErr                           (ucTestGroup_TestFunc)(ucTestGroup*);
uc_EXPORTED ucTestErr                           ucTestGroup_after_all_tests(ucTestGroup *p);
uc_EXPORTED ucTestErr                           ucTestGroup_after_each_test(ucTestGroup *p);
uc_EXPORTED ucTestErr                           ucTestGroup_before_all_tests(ucTestGroup *p);
uc_EXPORTED ucTestErr                           ucTestGroup_before_each_test(ucTestGroup *p);
uc_EXPORTED ucTestGroup_TestFunc**              ucTestGroup_get_tests(ucTestGroup *p);
uc_EXPORTED ucTestGroup*                        ucTestGroup_init(ucTestGroup *p, ucTestGroup_CallbackFunc *before_all_tests, ucTestGroup_CallbackFunc *after_all_tests, ucTestGroup_CallbackFunc *before_each_test, ucTestGroup_CallbackFunc *after_each_test, ucTestGroup_TestFunc **tests);
uc_EXPORTED ucTestErr                           ucTestGroup_run(ucTestGroup *p, ucTestState *state);
            struct                              ucTestGroup {
            ucTestGroup_CallbackFunc*           before_all_tests;
            ucTestGroup_CallbackFunc*           after_all_tests;
            ucTestGroup_CallbackFunc*           before_each_test;
            ucTestGroup_CallbackFunc*           after_each_test;
            ucTestGroup_TestFunc**              tests; };

typedef     void                                (ucTest_PrintFunc)(const char *str, void *state);
typedef     ucBool                              (ucTest_ExitFunc)(void* state);
uc_EXPORTED ucTestGroup**                       ucTest_get_groups(ucTest*);
uc_EXPORTED const char*                         ucTest_get_label(ucTest*);
uc_EXPORTED ucTestState*                        ucTest_get_state(ucTest*);
uc_EXPORTED ucTest*                             ucTest_init(ucTest*, const char *label, ucTestGroup **groups);
uc_EXPORTED ucTestErr                           ucTest_run(ucTest*);
uc_EXPORTED void                                ucTest_set_exit_func(ucTest*, ucTest_ExitFunc *value);
uc_EXPORTED void                                ucTest_set_exit_state(ucTest*, void *value);
uc_EXPORTED void                                ucTest_set_print_func(ucTest*, ucTest_PrintFunc *value);
uc_EXPORTED void                                ucTest_set_print_state(ucTest*, void *value);
            struct                              ucTest {
            const char*                         label;
            ucTestState                         state;
            ucTest_PrintFunc*                   print_func;
            void*                               print_state;
            ucTest_ExitFunc*                    exit_func;
            void*                               exit_state;
            ucTestGroup**                       groups; };

uc_EXPORTED ucTestGroup*                        ucArgOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucArgOptOwner_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucArgTok_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucArgTokOwner_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucBool_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdLine_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdLineApp_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdLineOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdLineToks_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdParser_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucCmdTok_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucSwitchOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucSwitchTok_tests_get_group(void);
uc_EXPORTED ucTestGroup*                        ucTok_tests_get_group(void);

uc_EXPORTED ucTest*                             ucTests_get_test(void);

#endif
