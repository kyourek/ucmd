#ifndef UCMD_INTERNAL_H
#define UCMD_INTERNAL_H

#include "ucmd.h"

extern const char               uc_cmd_terminator;
extern const char               ucTok_separator;
extern const char*              ucOpt_validation_err_invalid_argument_prefix;
extern const char*              ucOpt_validation_err_invalid_switch_prefix;
extern const char*              ucOpt_validation_err_invalid_switch_argument_prefix;

uc_EXPORTED ucOpt*              ucOpt_init(ucOpt*, const char *name, const char *desc, ucBool is_required);

uc_EXPORTED ucSwitchOpt*        ucSwitchOpt_init(ucSwitchOpt*, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, ucSwitchOpt *next);
uc_EXPORTED const char*         ucSwitchOpt_format_validation_err(ucSwitchOpt*, ucCmdLine *cmd, ucSwitchTok *switch_tok);

uc_EXPORTED ucArgOptOwner*      ucArgOptOwner_init(ucArgOptOwner*, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt);
uc_EXPORTED const char*         ucArgOptOwner_format_validation_err(ucArgOptOwner*, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);

uc_EXPORTED ucArgOpt*           ucArgOpt_init(ucArgOpt*, const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_numeric, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);
uc_EXPORTED const char*         ucArgOpt_format_validation_err(ucArgOpt*, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);

uc_EXPORTED char*               ucCmdLineApp_receive(ucCmdLineApp*);
uc_EXPORTED ucCmdLineOpt*       ucCmdLineOpt_init(ucCmdLineOpt*, ucCmdLineOpt_Func *func, void* state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt *next);
uc_EXPORTED const char*         ucCmdLineOpt_format_validation_err(ucCmdLineOpt*, ucCmdLine *cmd);

struct ucOpt {
    const char*                 name;
    const char*                 desc;
    ucBool                      is_required;
};

struct ucArgOpt {
    ucOpt                       base;
    ucBool                      is_numeric;
    ucArgOpt_NUMERIC_TYPE       numeric_min;
    ucArgOpt_NUMERIC_TYPE       numeric_max;
    int                         max_tok_count;
    int                         min_tok_count;
    ucArgOpt*                   next;
};

struct ucCmdParser {
    ucCmdTok*(*                 parse)(ucCmdParser *p, char *cmd);
};

struct ucArgOptOwner {
    ucOpt                       base;
    ucArgOpt*                   arg_opt;
};

struct ucSwitchOpt {
    ucArgOptOwner               base;
    ucSwitchOpt*                next;
};

struct ucCmdLineOpt {
    ucArgOptOwner               base;
    ucCmdLineOpt_Func*          func;
    void*                       state;
    ucSwitchOpt*                switch_opt;
    ucCmdLineOpt*               next;
};

struct ucCmdLine {
    ucCmdTok*                   cmd_tok;
    ucCmdLine_TransmitFunc*     transmit;
    ucCmdLine_IsCancelledFunc*  is_cancelled;
    void*                       transmit_state;
    void*                       is_cancelled_state;
    char                        response[ucCmdLine_RESPONSE_LENGTH];
	char                        response_buffer[ucCmdLine_RESPONSE_LENGTH];
    ucBool                      is_quiet;
};

struct ucCmdLineApp {
    ucCmdLine*                  cmd;
    ucCmdParser*                cmd_parser;
    ucCmdLineApp_ReceiveFunc*   receive;
    void*                       receive_state;
    ucErr(*                     run)(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt);
    const char*                 help_command;
    const char*                 quit_command;
    const char*                 escape_response;
    char                        cmd_buf[ucCmdLineApp_CMD_BUF_LENGTH + 1];
};

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
