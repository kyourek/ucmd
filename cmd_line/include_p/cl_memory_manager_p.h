#ifndef CL_MEMORY_MANAGER_P_H
#define CL_MEMORY_MANAGER_P_H

#define CL_MEMORY_MANAGER_INIT(type, count) \
    typedef struct cl_memory_manager_instance { \
        type s; \
        int in_use; \
    } cl_memory_manager_instance; \
\
    static cl_memory_manager_instance cl_memory_manager_instances[count]; \
\
    static type *cl_memory_manager_create(void) { \
        int i; \
        cl_memory_manager_instance *inst; \
        for (i = 0; i < count; i++) { \
            inst = &cl_memory_manager_instances[i]; \
            if (!inst->in_use) { \
                inst->in_use = 1; \
                return &inst->s; \
            } \
        } \
        return NULL; \
    } \
\
    static void cl_memory_manager_destroy(type *p) { \
        int i; \
        cl_memory_manager_instance *inst; \
        for (i = 0; i < count; i++) { \
            inst = &cl_memory_manager_instances[i]; \
            if (p == (&inst->s)) { \
                inst->in_use = 0; \
            } \
        } \
    } \

#endif
