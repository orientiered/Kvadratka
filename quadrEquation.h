/// @file
/// @brief Header with structs for quadratic equation

#ifndef QUADR_EQUATION_H
#define QUADR_EQUATION_H

#include <math.h>

/// @brief Exit codes used in solution_t struct
/// @see solution
enum solutionCode {
    ZERO_ROOTS,         ///< 0 roots
    ONE_ROOT,           ///< 1 root
    TWO_ROOTS,          ///< 2 roots
    INF_ROOTS,          ///< infinity roots, 0 = 0
    BAD_INPUT,          ///< When input is inf of Nan
    BLANK_ROOT = -1     ///< This code is used when solution_t is initialized
};


/*!
    \brief Struct that stores solutions
*/
typedef struct solution{
    enum solutionCode code; ///< enum with exit codes; in basic cases = number of roots
    double x1;              ///< first root
    double x2;              ///< second root
} solution_t;

const solution_t BLANK_SOLUTION = {BLANK_ROOT, NAN, NAN};


/// @brief Struct that stores coeffs and answers of quadratic equation
typedef struct quadraticEquation {
    double a, b, c;     ///< Coefficients of quadratic polynomial
    solution_t answer;  ///< structure with exit code and answers
} quadraticEquation_t;

const quadraticEquation_t BLANK_QUADRATIC_EQUATION = {NAN, NAN, NAN, BLANK_SOLUTION};

#endif
