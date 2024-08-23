/// @file
/// functions to run unitTests

#ifndef UNIT_TESTER_H
#define UNIT_TESTER_H

/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; //< Struct with test coefficients
    solution_t expectedData;       //< Struct with expected solutions
} unitTest_t;


/*!
    @brief Runs unit-tests

    @return error code

    Expects testData.h to be included
*/
enum error unitTesting();


/*!
    @brief Runs exactly one test

    @param[in] test Struct with test data and expected data

    @return Enum with error code
*/
enum error runTest(unitTest_t test);

#endif
