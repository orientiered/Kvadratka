#ifndef ERROR_H
#define ERROR_H

#define PROPAGATE_ERROR(result)                         \
        do{                                             \
            enum error res = result;                    \
            if (res == BAD_EXIT || res == FAIL)         \
            {                                           \
                printf("Error on line %d\n", __LINE__); \
                return res;                             \
            }                                           \
        }while(0)
//__FUNCTION__ __PRETTY_FUNCTION__ __FILE__

/// @brief Error codes which can be used in many functions
enum error {
    BAD_EXIT,           ///< Function handled fail inside it correctly
    GOOD_EXIT,          ///< Function worked correctly and all is as expected
    STRANGE_EXIT,       ///< Exit in condinitions that can't be reached
    FAIL,               ///< Emergency exit
    BLANK               ///< Empty error

};

#endif
