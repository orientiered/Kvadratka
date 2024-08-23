#include <stdio.h>
#include <string.h>

#include "error.h"
#include "quadrEquation.h"
#include "colors.h"
#include "quadraticSolver.h"
#include "quadraticPrinter.h"
#include "unitTester.h"
#include "utils.h"

#include "testData.h"


enum error unitTesting(int silent) {
    #ifndef TEST_DATA_INCLUDED
        printf("Include test data\n");
        return BAD_EXIT;
    #else
    const int testSize = sizeof(testData) / sizeof(unitTest_t);

    for (int testIndex = 0; testIndex < testSize; testIndex++) {
        if (runTest(testData[testIndex]) != GOOD_EXIT) {
            printf(RED_BKG "UNIT TESTING FAILED on test %d" RESET_C "\n", testIndex + 1);
            return BAD_EXIT;
        }
        else if (!silent) {
            printf(GREEN_BKG "Test #%d passed" RESET_C "\n", testIndex+1);
        }
    }

    #endif
    return GOOD_EXIT;
}


enum error unitTestingFile(const char name[], int silent) {
    FILE* testsF = fopen(name, "r");
    if (!testsF) {
        printf("Can't read file %s\n", name);
        fclose(testsF);
        return FAIL;
    }

    int testCount = 0;
    if (fscanf(testsF, " %d ", &testCount) != 1) {
        printf("Can't read number of tests\n");
        fclose(testsF);
        return BAD_EXIT;
    };

    for (int testIndex = 0; testIndex < testCount; testIndex++) {
        unitTest_t test = BLANK_TEST;
        enum error readStatus = readUnitTest(testsF, &test);
        if (readStatus != GOOD_EXIT) {
            printf("Can't read test #%d\n", testIndex+1);
            fclose(testsF);
            return readStatus;
        }

        if (runTest(test) != GOOD_EXIT) {
            printf(RED_BKG "UNIT TESTING FAILED on test %d" RESET_C "\n", testIndex + 1);
            fclose(testsF);
            return BAD_EXIT;
        }
        else if (!silent) {
            printf(GREEN_BKG "Test #%d passed" RESET_C "\n", testIndex+1);
        }
    }
    fclose(testsF);
    return GOOD_EXIT;
}


enum error readUnitTest(FILE* testsF, unitTest_t* test) {
    MY_ASSERT(testsF, return FAIL);

    const int MAX_LEN = 100;
    char solutionStr[MAX_LEN] = {};
    if (fscanf(testsF, " %lg %lg %lg %s %lg %lg",
         &test->inputData.a, &test->inputData.b, &test->inputData.c,
         solutionStr, &test->expectedData.x1, &test->expectedData.x2) != 6)
         return BAD_EXIT;

    if (parseSolutionCode(solutionStr, &test->expectedData.code) != GOOD_EXIT) {
        printf("Can't parse solutionCode enum\n");
        return BAD_EXIT;
    }

    return GOOD_EXIT;
}


enum error parseSolutionCode(const char solutionStr[], enum solutionCode* code) {
    int tempCode = 0;
    const int ENUM_SIZE = 6;
    const char *literals[ENUM_SIZE] = \
        {"BLANK_ROOT", "ZERO_ROOTS", "ONE_ROOT", "TWO_ROOTS", "INF_ROOTS", "BAD_INPUT"};

    if (sscanf(solutionStr, " %d ", &tempCode) == 1) {
        *code = (enum solutionCode) tempCode;
        return GOOD_EXIT;
    } else {
        int counter = -1;
        for (int i = 0; i < ENUM_SIZE; i++, counter++) {
            if (!strcmp(solutionStr, literals[i])) {
                *code = (enum solutionCode) counter;
                return GOOD_EXIT;
            }
        }
    }
    return BAD_EXIT;
}


enum error runTest(unitTest_t test) {
    solveEquation(&test.inputData);
    solution_t result = test.inputData.answer;

    if (result.code != test.expectedData.code) { //checking exit code first
        printKvadr(&test.inputData); //print equation
        printf(RED_BKG "Exit code doesn't match: " GREEN_BKG "expected %d, " CYAN_BKG "got %d" RESET_C "\n",
                test.expectedData.code, result.code);
        return BAD_EXIT;
    } else {
        switch (result.code) {
            case INF_ROOTS:
            case BLANK_ROOT:
            case BAD_INPUT:
            case ZERO_ROOTS:
                return GOOD_EXIT;
            case ONE_ROOT:
                if(cmpDouble(result.x1, test.expectedData.x1) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf(RED_BKG "Answers doesn't match: " GREEN_BKG "expected x = %lg, " CYAN_BKG "got x = %lg" RESET_C "\n",
                    test.expectedData.x1, result.x1);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            case TWO_ROOTS:
                if (result.x1 > result.x2)
                    SWAP_DOUBLE(result.x1, result.x2);
                if (cmpDouble(result.x1, test.expectedData.x1) != 0 || cmpDouble(result.x2, test.expectedData.x2) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf(RED_BKG "Answers doesn't match: " GREEN_BKG "expected x1 = %lg, x2 = %lg" RESET_C "\n"
                            CYAN_BKG "Got x1 = %lg, x2 = %lg" RESET_C "\n",
                            test.expectedData.x1, test.expectedData.x2, result.x1, result.x2);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            default:
                return BAD_EXIT;
        }
    }
    return STRANGE_EXIT;
}
