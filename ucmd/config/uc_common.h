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

#endif
