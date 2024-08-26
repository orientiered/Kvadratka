/// @file
/// @brief Functions to solve quadratic equation

#ifndef QUADRATIC_SOLVER
#define QUADRATIC_SOLVER


/*!
 *  @brief solves quadratic equation
 *
 *  @param[in, out] equation Pointer to struct that holds coeffs and answers
 *
 *  @returns Enum with error code
 *  In current implementation returns fail only if input is nan of inf
 *
 *  Solves quadratic equation in form ax^2 + bx + c = 0 <br>
 *  Fixes -0 in answer
*/
enum error solveEquation(quadraticEquation_t* equation);

#endif
