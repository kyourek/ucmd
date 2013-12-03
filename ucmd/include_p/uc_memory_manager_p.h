#ifndef UC_MEMORY_MANAGER_P_H
#define UC_MEMORY_MANAGER_P_H

#define UC_MEMORY_MANAGER_INIT(type, count) \
    typedef struct uc_memory_manager_instance { \
        type s; \
        int in_use; \
    } uc_memory_manager_instance; \
\
    static uc_memory_manager_instance uc_memory_manager_instances[count]; \
\
    static type *uc_memory_manager_create(void) { \
        int i; \
        uc_memory_manager_instance *inst; \
        for (i = 0; i < count; i++) { \
            inst = &uc_memory_manager_instances[i]; \
            if (!inst->in_use) { \
                inst->in_use = 1; \
                return &inst->s; \
            } \
        } \
        return NULL; \
    } \
\
    static void uc_memory_manager_destroy(type *p) { \
        int i; \
        uc_memory_manager_instance *inst; \
        for (i = 0; i < count; i++) { \
            inst = &uc_memory_manager_instances[i]; \
            if (p == (&inst->s)) { \
                inst->in_use = 0; \
            } \
        } \
    } \

#endif
