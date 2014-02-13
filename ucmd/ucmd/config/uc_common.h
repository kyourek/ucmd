#ifndef UC_COMMON_H
#define UC_COMMON_H

/* This file should be in the include path
   of the compiler. It provides implementation-
   specific configuration for the program. */
#include "uc_config.h"

/* Prepend extern "C" if we're in a C++
   compiler. */
#ifdef __cplusplus
#define UC_EXTERN_C extern "C"
#else
#define UC_EXTERN_C
#endif

/* Default to setting UC_EXPORTED to the result
   of our extern "C" check. */
#ifndef UC_EXPORTED
#define UC_EXPORTED UC_EXTERN_C
#endif

/* Sets the size of the command buffer when using
   the command-line application framework. All
   entered commands must have a size equal to or
   less than this buffer's size. */
#ifndef UC_CMD_LINE_APP_CMD_BUF_LENGTH
#define UC_CMD_LINE_APP_CMD_BUF_LENGTH 200
#endif

/* Sets the size of the command response buffer.
   All response strings must have a size equal to
   or less than the size of this buffer to avoid
   truncation. */
#ifndef UC_CMD_LINE_RESPONSE_LENGTH
#define UC_CMD_LINE_RESPONSE_LENGTH 200
#endif

/* Sets the number of available command options.
   The number of created command options must be
   equal to or less than this number. */
#ifndef UC_CMD_LINE_OPT_COUNT
#define UC_CMD_LINE_OPT_COUNT 10
#endif

/* Sets the number of available switch options.
   The number of created switch options must be
   equal to or less than this number. */
#ifndef UC_SWITCH_OPT_COUNT
#define UC_SWITCH_OPT_COUNT 50
#endif

/* Sets the number of available argument options.
   This is the total number of options available
   to commands and switches, combined. */
#ifndef UC_ARG_OPT_COUNT
#define UC_ARG_OPT_COUNT 50
#endif

/* Sets the maximum expected length of a single
   token in a command line. */
#ifndef UC_TOK_LENGTH_MAX
#define UC_TOK_LENGTH_MAX UC_CMD_LINE_APP_CMD_BUF_LENGTH
#endif

#endif
