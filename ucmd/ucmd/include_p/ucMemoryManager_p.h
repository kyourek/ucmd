#ifndef UCMEMORYMANAGER_P_H
#define UCMEMORYMANAGER_P_H

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
#define ucMemoryManager_INIT(TYPE, COUNT) \
    typedef struct ucMemoryManager_Instance { \
        TYPE inst; \
        char used; \
    } ucMemoryManager_Instance; \
\
    static ucMemoryManager_Instance ucMemoryManager_Instances[COUNT] = { 0 }; \
\
    static TYPE *ucMemoryManager_create(void) { \
        int i; \
        ucMemoryManager_Instance *inst; \
        for (i = 0; i < COUNT; i++) { \
            inst = &ucMemoryManager_Instances[i]; \
            if (inst->used == 0) { \
                inst->used = 1; \
                return &inst->inst; \
            } \
        } \
        return NULL; \
    } \
\
    static void ucMemoryManager_destroy(TYPE *p) { \
        int i; \
        ucMemoryManager_Instance *inst; \
        for (i = 0; i < COUNT; i++) { \
            inst = &ucMemoryManager_Instances[i]; \
            if (p == (&inst->inst)) { \
                inst->used = 0; \
            } \
        } \
    } \

#endif
