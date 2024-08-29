/// @file
/// @brief Helper functions

#ifndef UTILS_H
#define UTILS_H


/*!
    @brief Macro that swaps two numbers of specified type

    @param[in] a First number
    @param[in] b Second number
    @param[in] TYPE Type of @a a and @a b
*/
#define SWAP(a, b, TYPE)                \
            do{                         \
                TYPE c = (b);           \
                (b) = (a);              \
                (a) = (c);              \
            }while(0)


const double EPSILON = 1e-9; //constant for comparing floats


/*!
    @brief Fixes -0

    @param[in] num Number to fix

    @return Fixed number
*/
double fixMinusZero(double num); //fix -0

/*!
    @brief Safe double comparison

    @param[in] a First number
    @param[in] b Second number

    @return -1 if a < b, 0 if a == b, 1 if a > b

    @see EPSILON

    Using EPSILON constant to compare
*/
int cmpDouble(const double a, const double b);


/*!
    @brief Checks double on zero

    @return 1 if a == 0 else 0
*/
int isZero(const double a);

/*!
    @brief Checks double on NaN

    @return 1 if number is NaN, 0 else

    Uses bit operations and casts number to unsigned long long
*/
int myIsNan(double a);


/*!
    @brief Checks double on INFINITY

    @return 1 if number is +-INFINITY, 0 else

    Uses bit operations and casts number to unsigned long long
*/
int myIsInf(double a);


/*!
    @brief Swaps two variables of any type

    @param[in] a Pointer to first variable
    @param[in] b Pointer to second variable
    @param[in] size Size of variables in bytes

    Variables a and b must be the same type <br>
    BUT you can use it to swap anything, for example <br>
    two arrays <br>
    Under the hood it swaps values byte by byte
*/
void swap(void *a, void *b, size_t size);
#endif
