/// @file
/// @brief Struct and macros for error handling

#ifndef ERROR_H
#define ERROR_H

/*!
    @brief if expr is bad error, returns expr and prints where exception occured
*/
#include "colors.h"

/// @brief
#define PROPAGATE_ERROR(expr)                                                                               \
        do{                                                                                                 \
            enum error res = expr;                                                                          \
            if (res == BAD_EXIT || res == FAIL)                                                             \
            {                                                                                               \
                printf("Error. File: %s, function: %s, line: %d\n", __FILE__, __FUNCTION__, __LINE__);      \
                return res;                                                                                 \
            }                                                                                               \
        }while(0)
//__FUNCTION__ __PRETTY_FUNCTION__ __FILE__



/*!
    @brief if expr is false, executes run
*/
#define MY_ASSERT(expr, run)                                                                                            \
        do {                                                                                                            \
            if (!(expr)) {                                                                                              \
                printf(RED "Assertion failed:\n\t{" #expr "}\n" RESET_C);                                               \
                printf(RED "File: %s, function: %s, line: %d\n" RESET_C, __FILE__, __PRETTY_FUNCTION__, __LINE__);      \
                do {                                                                                                    \
                    run;                                                                                                \
                } while (0);                                                                                            \
            }                                                                                                           \
        }while(0)


/// @brief Error codes which can be used in many functions
enum error {
    BAD_EXIT,           ///< Function handled fail inside it correctly
    GOOD_EXIT,          ///< Function worked correctly and all is as expected
    STRANGE_EXIT,       ///< Exit in condinitions that can't be reached
    FAIL,               ///< Emergency exit
    BLANK               ///< Empty error
};

#endif
