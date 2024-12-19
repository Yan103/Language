/*!
    \file
    File with defines
*/

#ifndef CONFIG_H
#define CONFIG_H

/// @brief Macros for stack initialization
#define STACK_INIT(st) StackInit(__FILE__, __func__, __LINE__, #st)

/// Proper release of dynamic memory
#define FREE(object) { free(object); (object) = NULL; }

//* ========== IMPORTANT ==========
//* if you run the program in DEBUG mode (with the flag of the same name), additional information will be checked
//* else all these checks will be disabled
#ifdef DEBUG
    /// @brief Macros for Dumping information about stack
    #define STACK_DUMP(st) { StackDump(st, __FILE__, __func__, __LINE__); }
    /// @brief Macros for my assert function
    #define ASSERT(condition, text_error) my_assert(condition, text_error, __FILE__, __func__, __LINE__)
    /// @brief Macros for my stack assert function
    #define STACK_ASSERT(st) {                                    \
    if (!StackOk(st)) {                                           \
        STACK_DUMP(st);                                           \
        assert(0);                                                \
    }                                                             \
}
    /// @brief Macros for creating debug information about stack
    #define DEBUG_INFO(st, file, func, line, var_name) {          \
    (st)->debug_info = (DebugInfo*) calloc(1, sizeof(DebugInfo)); \
    ASSERT((st)->debug_info != NULL, ERR_MSG_NULL_PTR);           \
    (st)->debug_info->filename = file;                            \
    (st)->debug_info->func = func;                                \
    (st)->debug_info->line = line;                                \
    (st)->debug_info->var_name = var_name;                        \
}
#else
    /// @brief Macros for Dumping information about stack (empty in N_DEBUG mode)
    #define STACK_DUMP(st) {}
    /// @brief Macros for my stack assert function (empty in N_DEBUG mode)
    #define STACK_ASSERT(st) {}
    /// @brief Macros for creating debug information about stack (empty in N_DEBUG mode)
    #define DEBUG_INFO(st, file, func, line, var_name) {}
    /// @brief Macros for my assert function (empty in N_DEBUG mode)
    #define ASSERT(condition, text_error) {}
#endif

#endif // CONFIG_H