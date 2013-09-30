#ifndef CL_COMMON_H
#define CL_COMMON_H

#include "cl_config.h"

/* Definitions for command, argument, and
   token sizes. */
#ifndef CL_CMD_LEN_MAX
#define CL_CMD_LEN_MAX                  200
#endif
#ifndef CL_TOK_LEN_MAX
#define CL_TOK_LEN_MAX                  CL_CMD_LEN_MAX
#endif
#ifndef CL_CMD_COUNT_MAX
#define CL_CMD_COUNT_MAX                5
#endif
#ifndef CL_ARG_COUNT_MAX
#define CL_ARG_COUNT_MAX                5
#endif
#ifndef CL_SWITCH_COUNT_MAX
#define CL_SWITCH_COUNT_MAX             5
#endif
#ifndef CL_RESPONSE_LEN_MAX
#define CL_RESPONSE_LEN_MAX             1000
#endif

/* Prepend extern "C" if we're in a C++
   compiler. */
#ifdef __cplusplus
#define CL_EXTERN_C extern "C"
#else
#define CL_EXTERN_C
#endif

/* Default to setting CL_EXPORTED to the result
   of our extern "C" check. */
#ifndef CL_EXPORTED
#define CL_EXPORTED CL_EXTERN_C
#endif

#endif
