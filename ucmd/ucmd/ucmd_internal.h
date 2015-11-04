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

#ifndef     UCMD_INTERNAL_H
#define     UCMD_INTERNAL_H

#include    <assert.h>
#include    "ucmd.h"

extern      const char*                         ucOpt_validation_err_invalid_argument_prefix;
extern      const char*                         ucOpt_validation_err_invalid_switch_prefix;
extern      const char*                         ucOpt_validation_err_invalid_switch_argument_prefix;

            struct                              ucCmdParser {
            ucCmdTok*                           (*parse)(ucCmdParser *p, char *cmd); };

uc_EXPORTED ucOpt*                              ucOpt_init(ucOpt*, const char *name, const char *desc, ucBool is_required);
            struct                              ucOpt {
            const char*                         name;
            const char*                         desc;
            ucBool                              is_required; };

uc_EXPORTED const char*                         ucArgOpt_format_validation_err(ucArgOpt*, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);
uc_EXPORTED ucArgOpt*                           ucArgOpt_init(ucArgOpt*, const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_boolean, ucBool is_numeric, ucBool is_integer, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);
            struct                              ucArgOpt {
            ucOpt                               base;
            ucBool                              is_boolean;
            ucBool                              is_numeric;
            ucBool                              is_integer;
            ucArgOpt_NUMERIC_TYPE               numeric_min;
            ucArgOpt_NUMERIC_TYPE               numeric_max;
            int                                 max_tok_count;
            int                                 min_tok_count;
            ucArgOpt*                           next; };            

uc_EXPORTED const char*                         ucArgOptOwner_format_validation_err(ucArgOptOwner*, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);
uc_EXPORTED ucArgOptOwner*                      ucArgOptOwner_init(ucArgOptOwner*, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt);
            struct                              ucArgOptOwner {
            ucOpt                               base;
            ucArgOpt*                           arg_opt; };

uc_EXPORTED const char*                         ucSwitchOpt_format_validation_err(ucSwitchOpt*, ucCmdLine *cmd, ucSwitchTok *switch_tok);
uc_EXPORTED ucSwitchOpt*                        ucSwitchOpt_init(ucSwitchOpt*, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, ucSwitchOpt *next);
            struct                              ucSwitchOpt {
            ucArgOptOwner                       base;
            ucSwitchOpt*                        next; };

uc_EXPORTED ucBool                              ucCmdLine_handle_invalid_command(ucCmdLine*, const char *invalid_command);
uc_EXPORTED ucCmdLine*                          ucCmdLine_init(ucCmdLine*);
            struct                              ucCmdLine {
            ucCmdTok*                           cmd_tok;
            ucCmdLine_TransmitFunc*             transmit;
            ucCmdLine_IsCanceledFunc*           is_canceled;
            ucCmdLine_HandleInvalidCommandFunc* handle_invalid_command;
            void*                               transmit_state;
            void*                               is_canceled_state;
            void*                               handle_invalid_command_state;
            const char*                         response_terminator;
            const char*                         command_acknowledgment;
            char                                response[ucCmdLine_RESPONSE_SIZE];
	        char                                response_buffer[ucCmdLine_RESPONSE_SIZE];
            ucBool                              is_quiet; };

uc_EXPORTED const char*                         ucCmdLineOpt_format_validation_err(ucCmdLineOpt*, ucCmdLine *cmd);
uc_EXPORTED ucCmdLineOpt*                       ucCmdLineOpt_init(ucCmdLineOpt*, ucCmdLineOpt_Func *func, void* state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt *next);
            struct                              ucCmdLineOpt {
            ucArgOptOwner                       base;
            ucCmdLineOpt_Func*                  func;
            void*                               state;
            ucSwitchOpt*                        switch_opt;
            ucCmdLineOpt*                       next; };

uc_EXPORTED ucCmdLineApp*                       ucCmdLineApp_init(ucCmdLineApp*, ucCmdParser*, ucCmdLine*);
uc_EXPORTED char*                               ucCmdLineApp_receive(ucCmdLineApp*);
            struct                              ucCmdLineApp {
            ucCmdLine*                          cmd;
            ucCmdParser*                        cmd_parser;
            ucCmdLineApp_ReceiveFunc*           receive;
            void*                               receive_state;
            ucErr                               (*run)(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt);
            const char*                         help_command;
            const char*                         quit_command;
            const char*                         escape_response;            
            char                                cmd_str[ucCmdLineApp_CMD_STR_SIZE + 1]; };

/* 
 * Summary:
 *   Defines a macro that is used to provide a dynamic
 *   feel to static-memory allocation. Calling it in a
 *   source file exposes functions to create and destroy
 *   objects of the specified type.
 * Parameters:
 *   TYPE: The type of the object instances that are
 *         created and destroyed using the manager.
 *   COUNT: The number of object instances that is
 *          available to the program. Once this number
 *          is exceeded, NULL is returned from the
 *          create function until instances are released
 *          using the destroy function.
 */
#define ucMemoryManager_INIT(TYPE, COUNT)                                           \
    typedef struct ucMemoryManager_Instance {                                       \
        TYPE inst;                                                                  \
        char used;                                                                  \
    } ucMemoryManager_Instance;                                                     \
                                                                                    \
    static ucMemoryManager_Instance ucMemoryManager_Instances[COUNT] = { 0 };       \
                                                                                    \
    static TYPE *ucMemoryManager_create(void) {                                     \
        int i;                                                                      \
        ucMemoryManager_Instance *inst;                                             \
        for (i = 0; i < COUNT; i++) {                                               \
            inst = &ucMemoryManager_Instances[i];                                   \
            if (inst->used == 0) {                                                  \
                inst->used = 1;                                                     \
                return &inst->inst;                                                 \
            }                                                                       \
        }                                                                           \
        return NULL;                                                                \
    }                                                                               \
                                                                                    \
    static void ucMemoryManager_destroy(TYPE *p) {                                  \
        int i;                                                                      \
        ucMemoryManager_Instance *inst;                                             \
        for (i = 0; i < COUNT; i++) {                                               \
            inst = &ucMemoryManager_Instances[i];                                   \
            if (p == (&inst->inst)) {                                               \
                inst->used = 0;                                                     \
            }                                                                       \
        }                                                                           \
    }

#endif /* #ifndef UCMD_INTERNAL_H */
