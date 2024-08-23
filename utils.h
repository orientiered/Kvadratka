/// @file
/// Helper functions

#ifndef UTILS_H
#define UTILS_H

#define SWAP_DOUBLE(a, b)           \
            do{                     \
                double c = b;       \
                b = a;              \
                a = c;              \
            }while(0)


const double EPSILON = 1e-10; //constant for comparing floats

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
    Using EPSILON const to compare
*/
int cmpDouble(const double a, const double b);


/*!
    @brief Checks double on zero

    @return 1 if a == 0 else 0
*/
int isZero(const double a);

#endif
