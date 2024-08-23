/// @file
/// functions to run unitTests

#ifndef UNIT_TESTER_H
#define UNIT_TESTER_H

/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; //< Struct with test coefficients
    solution_t expectedData;       //< Struct with expected solutions
} unitTest_t;

const unitTest_t BLANK_TEST = {BLANK_QUADRATIC_EQUATION, BLANK_SOLUTION};


/*!
    @brief Runs unit-tests

    @param[in] silent: if 1 - unit testing should go silently, if 0 - print all messages

    @return error code

    Expects testData.h to be included
*/
enum error unitTesting(int silent);


/*!
    @brief Runs unit testing; reads them from file on fly

    @param[in] name Name of file with tests
    @param[in] silent: if 1 - unit testing should go silently, if 0 - print all messages
*/
enum error unitTestingFile(const char name[], int silent);


/*!
    @brief Tries to read unit test from file

    @param[in] testsF Pointer to file
    @param[in] test Pointer to unitTest struct

    @return Enum with error
*/
enum error readUnitTest(FILE* testsF, unitTest_t* test);


/*!
    @brief Tries to parse string with solutionCode enum

    Can parse literals and numbers
*/
enum error parseSolutionCode(const char solutionStr[], enum solutionCode* code);


/*!
    @brief Runs exactly one test

    @param[in] test Struct with test data and expected data

    @return Enum with error code
*/
enum error runTest(unitTest_t test);

#endif
