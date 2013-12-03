#ifndef UC_MEMORY_MANAGER_P_H
#define UC_MEMORY_MANAGER_P_H

#define UC_MEMORY_MANAGER_INIT(TYPE, COUNT) \
    typedef struct uc_memory_manager_instance { \
        TYPE inst; \
        char used; \
    } uc_memory_manager_instance; \
\
    static uc_memory_manager_instance uc_memory_manager_instances[COUNT] = { 0 }; \
\
    static TYPE *uc_memory_manager_create(void) { \
        int i; \
        uc_memory_manager_instance *inst; \
        for (i = 0; i < COUNT; i++) { \
            inst = &uc_memory_manager_instances[i]; \
            if (inst->used == 0) { \
                inst->used = 1; \
                return &inst->inst; \
            } \
        } \
        return NULL; \
    } \
\
    static void uc_memory_manager_destroy(TYPE *p) { \
        int i; \
        uc_memory_manager_instance *inst; \
        for (i = 0; i < COUNT; i++) { \
            inst = &uc_memory_manager_instances[i]; \
            if (p == (&inst->inst)) { \
                inst->used = 0; \
            } \
        } \
    } \

#endif
