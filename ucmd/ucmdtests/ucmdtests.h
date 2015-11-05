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

#define     ucFAIL()                                    do { return -1; } while (0)
#define     ucPASS()                                    do { return -0; } while (0)
#define     ucTRUE(TRUE)                                do { if (!(TRUE)) { printf("%s\n", #TRUE); return -1; } } while (0)
#define     uc_TEST(NAME)                               static int NAME(ucTestGroup *p) {
#define     uc_PASS                                     return 0; }
#define     uc_TRUE(EXPRESSION)                         do { if (!(EXPRESSION)) { return -1; } } while (0)

#define     uc_TEST_GROUP(NAME, SETUP, ...)             \
    static                                              \
    ucTestGroup_TestFunc*                               \
    ucTestGroup_TestFunc_ ## NAME[] = {                 \
        __VA_ARGS__,                                    \
        NULL };                                         \
    static                                              \
    ucTestGroup                                         \
    ucTestGroup_ ## NAME ## _s = {                      \
        #NAME,                                          \
        ucTestGroup_TestFunc_ ## NAME,                  \
        SETUP,                                          \
        NULL,                                           \
        NULL,                                           \
        NULL };                                         \
    ucTestGroup*                                        \
    ucTestGroup_ ## NAME = &                            \
    ucTestGroup_ ## NAME ## _s;                         \

#define     uc_TEST_RUNNER(NAME, ...)                   \
    static                                              \
    ucTestGroup**                                       \
    ucTestGroup_ ## NAME[] = {                          \
        __VA_ARGS__,                                    \
        NULL };                                         \
    static                                              \
    ucTestRunner                                        \
    ucTestRunner_s = {                                  \
        #NAME,                                          \
        NULL,                                           \
        ucTestGroup_ ## NAME,                           \
        NULL,                                           \
        NULL,                                           \
        0,                                              \
        0 };                                            \
    ucTestRunner*                                       \
    ucTestRunner_instance = &                           \
    ucTestRunner_s;                                     \

typedef     int                                         ucTestErr;
typedef     struct ucTestGroup                          ucTestGroup;
typedef     struct ucTestRunner                         ucTestRunner;

typedef     int                                         (ucTestGroup_TestFunc)(ucTestGroup*);
uc_EXPORTED const char*                                 ucTestGroup_get_name(ucTestGroup*);
uc_EXPORTED ucTestRunner*                               ucTestGroup_get_test_runner(ucTestGroup*);
uc_EXPORTED int                                         ucTestGroup_run(ucTestGroup*);
uc_EXPORTED void                                        ucTestGroup_set_name(ucTestGroup*, const char*);
uc_EXPORTED void                                        ucTestGroup_set_setup(ucTestGroup*, ucTestGroup_TestFunc*);
uc_EXPORTED void                                        ucTestGroup_set_test(ucTestGroup*, ucTestGroup_TestFunc**);
uc_EXPORTED void                                        ucTestGroup_set_test_runner(ucTestGroup*, ucTestRunner*);
uc_EXPORTED void                                        ucTestGroup_setup_test(ucTestGroup*, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after);
            struct                                      ucTestGroup {
            const char*                                 name;            
            ucTestGroup_TestFunc**                      test;
            ucTestGroup_TestFunc*                       setup;
            ucTestGroup_TestFunc*                       prior;
            ucTestGroup_TestFunc*                       after;
            ucTestRunner*                               test_runner; };

typedef     void                                        ucTestRunner_CloseFunc(void *state);
typedef     void                                        ucTestRunner_PrintFunc(const char *format, va_list va, void *state);
uc_EXPORTED void                                        ucTestRunner_inc_test_count(ucTestRunner*, ucBool test_passed);
uc_EXPORTED ucTestRunner*                               ucTestRunner_instance;
uc_EXPORTED void                                        ucTestRunner_print(ucTestRunner*, const char *format, va_list va);
uc_EXPORTED void                                        ucTestRunner_run(ucTestRunner*);
uc_EXPORTED void                                        ucTestRunner_set_close(ucTestRunner*, ucTestRunner_CloseFunc*);
uc_EXPORTED void                                        ucTestRunner_set_print(ucTestRunner*, ucTestRunner_PrintFunc*);
            struct                                      ucTestRunner {
            const char*                                 name;
            void*                                       state;
            ucTestGroup***                              group; 
            ucTestRunner_PrintFunc*                     print;
            ucTestRunner_CloseFunc*                     close;
            int                                         passed_test_count;
            int                                         failed_test_count; };

uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgOpt;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgOptOwner;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgTok;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucArgTokOwner;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucBool;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLine;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineApp;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineOpt;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdLineToks;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdParser;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucCmdTok;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucOpt;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucSwitchOpt;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucSwitchTok;
uc_EXPORTED ucTestGroup*                                ucTestGroup_ucTok;

#endif
