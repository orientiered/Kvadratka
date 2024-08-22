/// @file
/// Headers of structs and functions

#ifndef QUADRATIC_SOLVER
#define QUADRATIC_SOLVER


#define SWAP_DOUBLE(a, b)           \
            do{                     \
                double c = b;       \
                b = a;              \
                a = c;              \
            }while(0)


/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; //< Struct with test coefficients
    solution_t expectedData;       //< Struct with expected solutions
} unitTest_t;


/*!
    @brief Prints quadratic equation in nice format

    @param[in] equation Pointer to struct with coeffs

    @returns Enum with error code
    In current implementation return is always GOOD_EXIT

    Skips zero-coefs, and doesn't print 1
    Example: 0 1 -5
    => x - 5 = 0
*/
enum error printKvadr(const quadraticEquation_t* equation);


/*!
 *  @brief solves equation, prints some comments and returns struct with answers
 *
 *  @param[in, out] equation Pointer to struct that holds coeffs and answers
 *
 *  @returns Enum with error code
 *  In current implementation returns fail only if input is nan of inf
 *
 *  Solves quadratic equation in form ax^2 + bx + c = 0
 *  Prints comments, if there aren't any roots. Also fixes -0 in answer
*/
enum error solveQuadratic(quadraticEquation_t* equation);


/*!
    @brief Prints roots of solved equation

    @param[in] equation Pointer to struct that holds coeffs and answers
*/
void printAnswer(const quadraticEquation_t* equation);


/*!
    @brief Runs unit-tests

    @return error code
*/
enum error unitTesting();


/*!
    @brief Runs exactly one test

    @param[in] test Struct with test data and expected data

    @return 1 if test is passed, 0 if failed
*/
enum error runTest(unitTest_t test);


#endif
