/// @file
/// Functions to pretty print quadratic equation and it's answers

#ifndef QUADRATIC_PRINTER_H
#define QUADRATIC_PRINTER_H

/*!
    @brief Prints quadratic equation in nice format

    @param[in] equation Pointer to struct with coeffs

    @returns Enum with error code <br>
    In current implementation return is always GOOD_EXIT

    Skips zero-coeffs, and doesn't print 1 <br>
    Example: 0 1 -5 <br>
    => x - 5 = 0
*/
enum error printKvadr(const quadraticEquation_t* equation);



/*!
    @brief Prints roots of solved equation

    @param[in] equation Pointer to struct that holds coeffs and answers

    @return Enum with error code
*/
enum error printAnswer(const quadraticEquation_t* equation);
#endif
