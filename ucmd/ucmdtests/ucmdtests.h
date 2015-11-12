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

#define     uc_PASS                                 return 0; }
#define     uc_FAIL()                               do { return -1; } while (0)

#define     uc_TRUE(EXPRESSION)                     \
    do {                                            \
        if (!(EXPRESSION)) {                        \
            ucTestRunner_fail_true(                 \
                ucTestRunner_instance,              \
                #EXPRESSION);                       \
            return -1;                              \
        }                                           \
    }                                               \
    while (0)                                       \

#define     uc_NEAR(NUM_1, NUM_2, DELTA)            \
    do {                                            \
        if (                                        \
            (((NUM_1) > (NUM_2)) &&                 \
            (((NUM_1) - (NUM_2)) > DELTA)) ||       \
            (((NUM_2) > (NUM_1)) &&                 \
            (((NUM_2) - (NUM_1)) > DELTA))          \
        ) {                                         \
            ucTestRunner_fail_near(                 \
                ucTestRunner_instance,              \
                #NUM_1,                             \
                #NUM_2,                             \
                #DELTA);                            \
            return -1;                              \
        }                                           \
    }                                               \
    while (0)                                       \

#define     uc_TEST(NAME, ...)                      \
    static                                          \
    int                                             \
    NAME(ucTestGroup *p, ## __VA_ARGS__) {          \

#define     uc_CASE(TEST, NAME, ...)                \
    uc_TEST(TEST ## _ ## NAME)                      \
        return TEST(p, ## __VA_ARGS__);             \
    uc_PASS                                         \

#define     uc_TEST_GROUP(NAME, SETUP, ...)         \
    static                                          \
    ucTestGroup_TestFunc*                           \
    ucTestGroup_TestFunc_ ## NAME[] = {             \
        __VA_ARGS__,                                \
        NULL };                                     \
    static                                          \
    ucTestGroup                                     \
    ucTestGroup_ ## NAME ## _s = {                  \
        #NAME,                                      \
        ucTestGroup_TestFunc_ ## NAME,              \
        SETUP,                                      \
        NULL,                                       \
        NULL };                                     \
    ucTestGroup*                                    \
    ucTestGroup_ ## NAME = &                        \
    ucTestGroup_ ## NAME ## _s;                     \

#define     uc_TEST_RUNNER(NAME, ...)               \
    static                                          \
    ucTestGroup**                                   \
    ucTestGroup_ ## NAME[] = {                      \
        __VA_ARGS__,                                \
        NULL };                                     \
    static                                          \
    ucTestRunner                                    \
    ucTestRunner_s = {                              \
        #NAME,                                      \
        NULL,                                       \
        ucTestGroup_ ## NAME,                       \
        NULL,                                       \
        NULL,                                       \
        0,                                          \
        0 };                                        \
    ucTestRunner*                                   \
    ucTestRunner_instance = &                       \
    ucTestRunner_s;                                 \

typedef     struct ucTestGroup                      ucTestGroup;
typedef     struct ucTestRunner                     ucTestRunner;

typedef     int                                     (ucTestGroup_TestFunc)(ucTestGroup*);
uc_EXPORTED int                                     ucTestGroup_after(ucTestGroup*);
uc_EXPORTED const char*                             ucTestGroup_get_name(ucTestGroup*);
uc_EXPORTED ucTestGroup_TestFunc**                  ucTestGroup_get_test(ucTestGroup*);
uc_EXPORTED int                                     ucTestGroup_prior(ucTestGroup*);
uc_EXPORTED int                                     ucTestGroup_setup(ucTestGroup*);
uc_EXPORTED void                                    ucTestGroup_setup_test(ucTestGroup*, ucTestGroup_TestFunc *prior, ucTestGroup_TestFunc *after);
            struct                                  ucTestGroup {
            const char*                             name;
            ucTestGroup_TestFunc**                  test;
            ucTestGroup_TestFunc*                   setup;
            ucTestGroup_TestFunc*                   prior;
            ucTestGroup_TestFunc*                   after; };

typedef     void                                    ucTestRunner_CloseFunc(void *state);
typedef     void                                    ucTestRunner_PrintFunc(const char *format, va_list va, void *state);
uc_EXPORTED void                                    ucTestRunner_fail_near(ucTestRunner*, const char *num_1, const char *num_2, const char *delta);
uc_EXPORTED void                                    ucTestRunner_fail_true(ucTestRunner*, const char *assertion);
uc_EXPORTED ucTestRunner*                           ucTestRunner_instance;
uc_EXPORTED int                                     ucTestRunner_run(ucTestRunner*);
uc_EXPORTED void                                    ucTestRunner_set_close(ucTestRunner*, ucTestRunner_CloseFunc*);
uc_EXPORTED void                                    ucTestRunner_set_print(ucTestRunner*, ucTestRunner_PrintFunc*);
            struct                                  ucTestRunner {
            const char*                             name;
            void*                                   state;
            ucTestGroup***                          group; 
            ucTestRunner_PrintFunc*                 print;
            ucTestRunner_CloseFunc*                 close;
            int                                     passed_test;
            int                                     failed_test; };

uc_EXPORTED ucTestGroup*                            ucTestGroup_ucArgOpt;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucArgOptOwner;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucArgTok;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucArgTokOwner;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucBool;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucCmd;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucApp;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucCmdOpt;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucParser;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucCmdTok;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucOpt;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucSwitchOpt;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucSwitchTok;
uc_EXPORTED ucTestGroup*                            ucTestGroup_ucTok;

#endif
