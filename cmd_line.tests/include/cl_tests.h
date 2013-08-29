#ifndef CL_TESTS_H
#define CL_TESTS_H

#define cl_fail(message) do { return message; } while (0)
#define cl_assert(message, test) do { if (!(test)) return message; else cl_assertions++; } while (0)
#define cl_run_test(test) do { char *message = test(); cl_tests_run++; \
                          if (message) return message; } while (0)
extern int cl_tests_run;
extern int cl_assertions;

char *cl_run_all_tests(void);

#endif
