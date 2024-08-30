/// @file
/// @brief
#ifndef MAIN_H
#define MAIN_H

/*!
    @brief Prints help message and basic info about program

    @param[in] flags Array of flags

    Prints help if -h flag is activated, <br>
    Basic info about program if -s flag isn't activated
*/
void initPrint(argVal_t flags[]);


/*!
    @brief Runs unit tests

    @param[in] flags Array of flags

    Will run unit test from internal array or from specified in argv file if flags -u or -u and -f are activated
*/
enum error unitTester(argVal_t flags[]);


/*!
    @brief Contains all logic to solve quadratic equation if coefficients are given in argv

    This function is made only to make main simpler
*/
enum error solveCmd(argVal_t flags[], enum error* scanResult, quadraticEquation_t* equation);


/*!
    @brief Loop with all logic to solve quadratic equation many times

    Reads coefficients from console, solves equation, prints result, asks to solve again <br>
    This function is made only to make main simpler
*/
enum error solveLoop(argVal_t flags[], enum error* scanResult, quadraticEquation_t* equation);

#endif
