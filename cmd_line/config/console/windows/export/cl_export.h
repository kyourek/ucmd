#ifndef CL_EXPORT_H
#define CL_EXPORT_H

/* Include this file when compiling this program. */

/* Exported functions will be decorated with dllexport
   to make them available to external programs. */
#define CL_EXPORTED CL_EXTERN_C __declspec(dllexport)

#endif
