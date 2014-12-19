#ifndef UC_EXPORT_H
#define UC_EXPORT_H

/* Include this file when compiling this program. */

/* Exported functions will be decorated with dllexport
   to make them available to external programs. */
#define uc_EXPORTED uc_EXTERN_C __declspec(dllexport)

#endif
