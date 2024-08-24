/// @file
/// @brief functions to run unitTests

#ifndef UNIT_TESTER_H
#define UNIT_TESTER_H

/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; ///< Struct with test coefficients
    solution_t expectedData;       ///< Struct with expected solutions
} unitTest_t;

const unitTest_t BLANK_TEST = {BLANK_QUADRATIC_EQUATION, BLANK_SOLUTION}; /// Empty initializer for unitTest_t


/*!
    @brief Runs internal unit testing, if testData.h is included

    @param[in] silent: if 1 - unit testing should go silently, if 0 - print all messages

    @return Enum with error code

    Firstly checks if "testData.h" is included. <br>
    Structure of testData.h: <br>
        1. const array of unitTest_t with tests named internalTestData <br>
        2. const int internalTestSize
    If not, it will print warning message and return FAIL. <br>
    Then returns unitTesting() by calling it
*/
enum error unitTestingInternal(int silent);


/*!
    @brief Runs unit testing; reads them to temporary array

    @param[in] name Name of file with tests
    @param[in] silent: if 1 - unit testing should go silently, if 0 - print all messages

    @return Enum with error code

    Opens file with specified name. <br>
    Then it allocates memory for array and reads unit tests from file to it. <br>
    Runs unitTesting() and frees allocated memory. <br>
    If something goes wrong, it will print warning message and return FAIL or BAD_EXIT. <br>
    In other cases it will return GOOD_EXIT.
*/
error unitTestingFile(const char name[], int silent);


/*!
    @brief Runs unit testing; reads them from file on fly

    @param[in] name Name of file with tests
    @param[in] silent: if 1 - unit testing should go silently, if 0 - print all messages

    @return Enum with error code

    Deprecated. Added new function with the same name.
*/
enum error unitTestingFile_DEPRECATED(const char name[], int silent);


/*!
    @brief Tries to read unit test from file

    @param[in] testsF Pointer to file
    @param[in] test Pointer to unitTest struct

    @return Enum with error code

    Tries to read coefficients a, b, c of quadratic equation, <br>
    number of roots (supports literal names like ONE_ROOT), <br>
    expected roots x1, x2 <br>

    @see solutionCode_t
*/
enum error readUnitTest(FILE* testsF, unitTest_t* test);


/*!
    @brief Tries to parse string with solutionCode enum

    @param[in] solutionStr String with solutionCode
    @param[in] code Pointer to enum solutionCode

    @return Enum with error code

    Can parse literals and numbers, specified in solutionCode_t. <br>
    Examples of solutionStr: "3", "INF_ROOTS" <br>

*/
enum error parseSolutionCode(const char solutionStr[], enum solutionCode* code);


/*!
    @brief Runs exactly one test

    @param[in] test Struct with test data and expected data

    @return Enum with error code

    Solves equation with solveEquation() function. <br>
    Then checks result with expected data in this order: <br>
    1. Number of roots(solutionCode_t) <br>
    2. Roots <br>
    Function won't compare roots if they don't have practical sense (e.g. 0 roots or inf number of roots)
*/
enum error runTest(unitTest_t test);

#endif
